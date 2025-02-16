#include "benchmark.h"

class Pricer;  // Forward declaration

class Data {
public:
    virtual ~Data() = default;
    virtual double calculatePrice() const = 0;
    virtual double getCommonFactor() const { return commonFactor; }
    double calculatePriceImpl() const;
    explicit Data(Pricer* p) : pricer(p) {}
protected:
    double commonFactor = 0.5;
    Pricer* pricer;
};

class Pricer {
public:
    double calculatePrice(const Data* data) const { return data->calculatePrice(); }
};

class StockData : public Data {
public:
    StockData(Pricer* p) : Data(p), priceFactor(1.2) {}
    double calculatePrice() const override { return priceFactor * 1.1 + getCommonFactor(); }
private:
    double priceFactor;
};

class OptionData : public Data {
public:
    OptionData(Pricer* p) : Data(p), volatility(0.8) {}
    double calculatePrice() const override { return volatility * 2.5 + getCommonFactor(); }
private:
    double volatility;
};

double Data::calculatePriceImpl() const { return pricer->calculatePrice(this); }

int main() {
    Pricer pricer;

    std::vector<std::unique_ptr<Data>> dataSamples;
    for (size_t i = 0; i < SAMPLE_SIZE/2; ++i) {
        dataSamples.emplace_back(std::make_unique<StockData>(&pricer));
        dataSamples.emplace_back(std::make_unique<OptionData>(&pricer));
    }
    
    benchmark("Design 9: Virtual with Pricer", [&]() {
        for (const auto& data : dataSamples) {
            data->calculatePriceImpl();
        }
    }, ITERATIONS);

    return 0;
} 