#include "benchmark.h"
#include <variant>
#include <iostream>
#include <vector>

template <typename Derived, typename DataType>
class Pricer {
public:
    double calculatePrice(const DataType& data) const {
        return static_cast<const Derived*>(this)->calculatePriceImpl(data);
    }
};

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

// Forward declarations
class StockPricer;
class OptionPricer;

// Data class declarations
class StockData : public Data<StockData> {
public:
    StockData(StockPricer* p);
    double calculatePriceImpl() const;
    double getPriceFactor() const;
private:
    double priceFactor;
    StockPricer* pricer;
};

class OptionData : public Data<OptionData> {
public:
    OptionData(OptionPricer* p);
    double calculatePriceImpl() const;
    double getVolatility() const;
private:
    double volatility;
    OptionPricer* pricer;
};

// Pricer class definitions
class StockPricer : public Pricer<StockPricer, StockData> {
public:
    double calculatePriceImpl(const StockData& data) const {
        return data.getPriceFactor() * 1.1 + data.getCommonFactor();
    }
};

class OptionPricer : public Pricer<OptionPricer, OptionData> {
public:
    double calculatePriceImpl(const OptionData& data) const {
        return data.getVolatility() * 2.5 + data.getCommonFactor();
    }
};

// Data class method implementations
StockData::StockData(StockPricer* p) : priceFactor(1.2), pricer(p) {}
double StockData::calculatePriceImpl() const { return pricer->calculatePrice(*this); }
double StockData::getPriceFactor() const { return priceFactor; }

OptionData::OptionData(OptionPricer* p) : volatility(0.8), pricer(p) {}
double OptionData::calculatePriceImpl() const { return pricer->calculatePrice(*this); }
double OptionData::getVolatility() const { return volatility; }

using DataVariant = std::variant<StockData, OptionData>;

int main() {
    StockPricer stockPricer;
    OptionPricer optionPricer;
    std::vector<DataVariant> dataSamples;
    for (size_t i = 0; i < SAMPLE_SIZE/2; ++i) {
        dataSamples.emplace_back(StockData(&stockPricer));
        dataSamples.emplace_back(OptionData(&optionPricer));
    }
    
    // auto data = dataSamples[0];
    // auto price = std::visit([](auto&& arg) -> double { return arg.calculatePrice(); }, data);
    // std::cout << "Price: " << price << std::endl;

    benchmark("Design: CRTP with Pricer", [&]() {
        for (const auto& var : dataSamples) {
            std::visit([&](const auto& data) {
                data.calculatePrice();
            }, var);
        }
    }, ITERATIONS);

    return 0;
} 