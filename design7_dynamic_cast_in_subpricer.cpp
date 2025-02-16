#include "benchmark.h"

class StockPricer;
class OptionPricer;

class Data {
public:
    virtual ~Data() = default;
    virtual double getCommonFactor() const { return commonFactor; }
    virtual double calculatePrice() const = 0;
protected:
    double commonFactor = 0.5;
};

class StockData : public Data {
public:
    StockData(StockPricer* p) : Data(), pricer(p), priceFactor(1.2) {}
    double calculatePrice() const override;
    StockPricer* pricer;
    double priceFactor;
};

class OptionData : public Data {
public:
    OptionData(OptionPricer* p) : Data(), pricer(p), volatility(0.8) {}
    double calculatePrice() const override;
    OptionPricer* pricer;
    double volatility;
};

class StockPricer {
public:
    double calculatePrice(const Data* data) const { 
        if (auto* stock = dynamic_cast<const StockData*>(data)) {
            return stock->priceFactor * 1.1 + stock->getCommonFactor();
        }
        return 0.0;
    }
};

class OptionPricer {
public:
    double calculatePrice(const Data* data) const { 
        if (auto* option = dynamic_cast<const OptionData*>(data)) {
            return option->volatility * 2.5 + option->getCommonFactor();
        }
        return 0.0;
    }
};

double StockData::calculatePrice() const { return pricer->calculatePrice(this); }
double OptionData::calculatePrice() const { return pricer->calculatePrice(this); }

int main() {
    StockPricer stockPricer;
    OptionPricer optionPricer;
    
    std::vector<std::unique_ptr<Data>> dataSamples;
    for (size_t i = 0; i < SAMPLE_SIZE/2; ++i) {
        dataSamples.emplace_back(std::make_unique<StockData>(&stockPricer));
        dataSamples.emplace_back(std::make_unique<OptionData>(&optionPricer));
    }
    
    benchmark("Design 7: Dynamic cast in subpricer", [&]() {
        for (const auto& data : dataSamples) {
            data->calculatePrice();
        }
    }, ITERATIONS);

    return 0;
} 