#include "benchmark.h"
#include <variant>

template <typename Derived>
class Pricer;  // Forward declaration

template <typename Derived>
class Data {
public:
    double calculatePrice() const {
        return static_cast<const Derived*>(this)->calculatePriceImpl();
    }
    double getCommonFactor() const { return commonFactor; }
    double calculatePriceOutside() const {
        return pricer->calculatePrice(*static_cast<const Derived*>(this));
    }
    explicit Data(Pricer<Derived>* p) : pricer(p) {}
protected:
    double commonFactor = 0.5;
    Pricer<Derived>* pricer;
};

template <typename Derived>
class Pricer {
public:
    template <typename T>
    double calculatePrice(const T& data) const {
        return data.calculatePrice();  // Direct call with static dispatch
    }
};

class StockData : public Data<StockData> {
public:
    StockData(Pricer<StockData>* p) : Data<StockData>(p), priceFactor(1.2) {}
    double calculatePriceImpl() const {
        return priceFactor * 1.1 + getCommonFactor();
    }
    double getPriceFactor() const { return priceFactor; }
private:
    double priceFactor;
};

class OptionData : public Data<OptionData> {
public:
    OptionData(Pricer<OptionData>* p) : Data<OptionData>(p), volatility(0.8) {}
    double calculatePriceImpl() const {
        return volatility * 2.5 + getCommonFactor();
    }
    double getVolatility() const { return volatility; }
private:
    double volatility;
};

using DataVariant = std::variant<StockData, OptionData>;


int main() {
    Pricer<StockData> stockPricer;
    Pricer<OptionData> optionPricer;
    std::vector<DataVariant> dataSamples;
    for (size_t i = 0; i < SAMPLE_SIZE/2; ++i) {
        dataSamples.emplace_back(StockData(&stockPricer));
        dataSamples.emplace_back(OptionData(&optionPricer));
    }
    
    benchmark("Design 11: CRTP with Pricer", [&]() {
        for (const auto& var : dataSamples) {
            std::visit([&](const auto& data) {
                data.calculatePriceOutside();
            }, var);
        }
    }, ITERATIONS);

    return 0;
} 