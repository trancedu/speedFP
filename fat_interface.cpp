#include "benchmark.h"

class Data {
public:
    virtual ~Data() = default;
    virtual double getPrice() const = 0;
    virtual double getCommonFactor() const { return commonFactor; }
    virtual double getPriceFactor() const { return 0; }
    virtual double getVolatility() const { return 0; }
protected:
    double commonFactor = 0.5;
};

class StockData : public Data {
public:
    StockData() : priceFactor(1.2) {}
    double getPrice() const override { return priceFactor * 1.1 + getCommonFactor(); }
    double getPriceFactor() const override { return priceFactor; }
private:
    double priceFactor;
};

class OptionData : public Data {
public:
    OptionData() : volatility(0.8) {}
    double getPrice() const override { return volatility * 2.5 + getCommonFactor(); }
    double getVolatility() const override { return volatility; }
private:
    double volatility;
};

int main() {
    std::vector<std::unique_ptr<Data>> dataSamples;
    for (size_t i = 0; i < SAMPLE_SIZE/2; ++i) {
        dataSamples.emplace_back(std::make_unique<StockData>());
        dataSamples.emplace_back(std::make_unique<OptionData>());
    }
    
    benchmark("Design: Fat interface Virtual", [&]() {
        for (const auto& data : dataSamples) {
            data->getPrice();
        }
    }, ITERATIONS);

    return 0;
} 