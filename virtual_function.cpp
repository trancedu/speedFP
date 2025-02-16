#include "benchmark.h"

class Data {
public:
    virtual ~Data() = default;
    virtual double calculatePrice() const = 0;
    virtual double getCommonFactor() const { return commonFactor; }
protected:
    double commonFactor = 0.5;
};

class StockData : public Data {
public:
    StockData() : priceFactor(1.2) {}
    double calculatePrice() const override { return priceFactor * 1.1 + getCommonFactor(); }
private:
    double priceFactor;
};

class OptionData : public Data {
public:
    OptionData() : volatility(0.8) {}
    double calculatePrice() const override { return volatility * 2.5 + getCommonFactor(); }
private:
    double volatility;
};

int main() {
    std::vector<std::unique_ptr<Data>> dataSamples;
    for (size_t i = 0; i < SAMPLE_SIZE/2; ++i) {
        dataSamples.emplace_back(std::make_unique<StockData>());
        dataSamples.emplace_back(std::make_unique<OptionData>());
    }
    
    benchmark("Design: Virtual function", [&]() {
        for (const auto& data : dataSamples) {
            data->calculatePrice();
        }
    }, ITERATIONS);

    return 0;
} 