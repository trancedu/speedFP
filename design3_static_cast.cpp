#include "benchmark.h"

class Data {
public:
    virtual ~Data() = default;
    virtual bool isStock() const = 0;
    virtual double getCommonFactor() const { return commonFactor; }
protected:
    double commonFactor = 0.5;
};

class StockData : public Data {
public:
    StockData() : priceFactor(1.2) {}
    bool isStock() const override { return true; }
    double priceFactor;
};

class OptionData : public Data {
public:
    OptionData() : volatility(0.8) {}
    bool isStock() const override { return false; }
    double volatility;
};

class StaticPricer {
public:
    double calculatePrice(const Data* data) const {
        if (data->isStock()) {
            return static_cast<const StockData*>(data)->priceFactor * 1.1 + data->getCommonFactor();
        }
        return static_cast<const OptionData*>(data)->volatility * 2.5 + data->getCommonFactor();
    }
};

int main() {
    StaticPricer pricer;
    std::vector<std::unique_ptr<Data>> dataSamples;
    for (size_t i = 0; i < SAMPLE_SIZE/2; ++i) {
        dataSamples.emplace_back(std::make_unique<StockData>());
        dataSamples.emplace_back(std::make_unique<OptionData>());
    }
    
    benchmark("Design 3: Static cast with Pricer", [&]() {
        for (const auto& data : dataSamples) {
            pricer.calculatePrice(data.get());
        }
    }, ITERATIONS);

    return 0;
} 