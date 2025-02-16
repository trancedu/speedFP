#include "benchmark.h"
#include <variant>

template <typename Derived>
class Data {
public:
    double calculatePrice() const {
        return static_cast<const Derived*>(this)->calculatePriceImpl();
    }
    double getCommonFactor() const { return commonFactor; }
protected:
    double commonFactor = 0.5;
};

class StockData : public Data<StockData> {
public:
    StockData() : priceFactor(1.2) {}
    double calculatePriceImpl() const {
        return priceFactor * 1.1 + getCommonFactor();
    }
    double getPriceFactor() const { return priceFactor; }
private:
    double priceFactor;
};

class OptionData : public Data<OptionData> {
public:
    OptionData() : volatility(0.8) {}
    double calculatePriceImpl() const {
        return volatility * 2.5 + getCommonFactor();
    }
    double getVolatility() const { return volatility; }
private:
    double volatility;
};

using DataVariant = std::variant<StockData, OptionData>;

int main() {
    std::vector<DataVariant> dataSamples;
    for (size_t i = 0; i < SAMPLE_SIZE/2; ++i) {
        dataSamples.emplace_back(StockData{});
        dataSamples.emplace_back(OptionData{});
    }
    
    benchmark("Design 6: CRTP with variant", [&]() {
        for (const auto& var : dataSamples) {
            std::visit([](const auto& data) {
                data.calculatePrice();
            }, var);
        }
    }, ITERATIONS);

    return 0;
} 