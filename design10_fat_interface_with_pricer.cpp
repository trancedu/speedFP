#include "benchmark.h"

class Pricer;  // Forward declaration

class Data {
public:
    virtual ~Data() = default;
    double calculatePriceImpl() const;
    virtual double getCommonFactor() const { return commonFactor; }
    virtual double getPriceFactor() const { return 0; }
    virtual double getVolatility() const { return 0; }
    explicit Data(Pricer* p) : pricer(p) {}
protected:
    double commonFactor = 0.5;
    Pricer* pricer;
};

class StockData : public Data {
public:
    StockData(Pricer* p) : Data(p), priceFactor(1.2) {}
    double getPriceFactor() const override { return priceFactor; }
private:
    double priceFactor;
};

class OptionData : public Data {
public:
    OptionData(Pricer* p) : Data(p), volatility(0.8) {}
    double getVolatility() const override { return volatility; }
private:
    double volatility;
};

class Pricer {
public:
    virtual ~Pricer() = default;
    virtual double calculatePrice(const Data* data) const = 0;
};

class StockPricer : public Pricer {
public:
    double calculatePrice(const Data* data) const override {
        return data->getPriceFactor() * 1.1 + data->getCommonFactor(); 
    }
};

class OptionPricer : public Pricer {
public:
    double calculatePrice(const Data* data) const override {
        return data->getVolatility() * 2.5 + data->getCommonFactor();
    }
};

double Data::calculatePriceImpl() const { return pricer->calculatePrice(this); }

int main() {
    StockPricer stockPricer;
    OptionPricer optionPricer;
    std::vector<std::unique_ptr<Data>> dataSamples;
    for (size_t i = 0; i < SAMPLE_SIZE/2; ++i) {
        dataSamples.emplace_back(std::make_unique<StockData>(&stockPricer));
        dataSamples.emplace_back(std::make_unique<OptionData>(&optionPricer));
    }
    
    benchmark("Design 10: Fat Interface with Pricer", [&]() {
        for (const auto& data : dataSamples) {
            data->calculatePriceImpl();
        }
    }, ITERATIONS);

    return 0;
} 