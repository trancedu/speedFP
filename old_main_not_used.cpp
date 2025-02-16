#include <iostream>
#include <chrono>
#include <vector>
#include <memory>
#include <random>

// Add forward declaration for Data before Pricer
class Data;

class StockPricer;
class OptionPricer;
class Pricer;

// Base Pricer class for Designs 2-5
class Pricer {
public:
    virtual ~Pricer() = default;
    virtual double calculatePriceBaseDynamic(const Data* data) const = 0;
    virtual double calculatePriceBaseStatic(const Data* data) const = 0;
    virtual double calculatePriceFat(const Data* data) const = 0;
};

// Forward declarations for derived data classes
class StockData;
class OptionData;

class StockPricer : public Pricer {
public:
    double calculatePrice(const StockData* stock) const;
    double calculatePriceBaseDynamic(const Data* data) const override;
    double calculatePriceBaseStatic(const Data* data) const override;
    double calculatePriceFat(const Data* data) const override;
};

class OptionPricer : public Pricer {
public:
    double calculatePrice(const OptionData* option) const;
    double calculatePriceBaseDynamic(const Data* data) const override;
    double calculatePriceBaseStatic(const Data* data) const override; 
    double calculatePriceFat(const Data* data) const override;
};

// Base data class
class Data {
public:
    virtual ~Data() = default;
    virtual double calculatePrice() const = 0; // Method for design 1
    virtual double getCommonFactor() const { return commonFactor;}; // Method for design 5
    virtual double getPriceFactor() const { return 0;}; // Method for design 5
    virtual double getVolatility() const { return 0;}; // Method for design 5
    virtual double calculatePriceFat() const {
        return pricer->calculatePriceFat(this);
    }
    virtual double calculatePriceBaseDynamic() const {
        return pricer->calculatePriceBaseDynamic(this);
    }
    virtual double calculatePriceBaseStatic() const {
        return pricer->calculatePriceBaseStatic(this);
    }
    virtual double calculatePriceDerived() const = 0;
    explicit Data(Pricer* pricer) : pricer(pricer) {}
protected:
    double commonFactor = 0.5;
    Pricer* pricer;
};

// Derived data classes
class StockData : public Data {
public:
    double priceFactor;
    explicit StockData(double factor, Pricer* pricer, StockPricer* stockPricer) : Data(pricer), priceFactor(factor), stockPricer(stockPricer) {}
    double calculatePrice() const override { return priceFactor * 1.1; }
    double getPriceFactor() const override { return priceFactor; }
    double calculatePriceDerived() const override { return stockPricer->calculatePrice(this); }
    StockPricer* stockPricer;
};

class OptionData : public Data {
public:
    double volatility;
    explicit OptionData(double vol, Pricer* pricer, OptionPricer* optionPricer) : Data(pricer), volatility(vol), optionPricer(optionPricer) {}
    double calculatePrice() const override { return volatility * 2.5; }
    double getVolatility() const override { return volatility; }
    double calculatePriceDerived() const override { return optionPricer->calculatePrice(this); }
    OptionPricer* optionPricer;
};

// Benchmark function
template <typename Func>
void benchmark(const std::string& label, Func func, size_t iterations) {
    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < iterations; ++i) {
        func();
    }
    auto end = std::chrono::high_resolution_clock::now();
    
    // Calculate duration in nanoseconds
    auto total_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    double average_ns = static_cast<double>(total_ns) / iterations / iterations;
    
    std::cout << label << " - Average: " << average_ns << " ns/iter\n";
}

// Add method implementations after all classes are defined
double StockPricer::calculatePrice(const StockData* stock) const { 
    return stock->priceFactor * 1.1 + stock->getCommonFactor(); 
}

double StockPricer::calculatePriceBaseDynamic(const Data* data) const { 
    if (auto* stock = dynamic_cast<const StockData*>(data)) {
        return stock->priceFactor * 1.1 + stock->getCommonFactor();
    }
    return 0.0;
}

double StockPricer::calculatePriceBaseStatic(const Data* data) const { 
    return static_cast<const StockData*>(data)->priceFactor * 1.1 + data->getCommonFactor();
}

double StockPricer::calculatePriceFat(const Data* data) const {
    return data->getPriceFactor() * 1.1 + data->getCommonFactor();
}

double OptionPricer::calculatePrice(const OptionData* option) const { 
    return option->volatility * 2.5 + option->getCommonFactor(); 
}

double OptionPricer::calculatePriceBaseDynamic(const Data* data) const { 
    if (auto* option = dynamic_cast<const OptionData*>(data)) {
        return option->volatility * 2.5 + option->getCommonFactor();
    }
    return 0.0;
}

double OptionPricer::calculatePriceBaseStatic(const Data* data) const { 
    return static_cast<const OptionData*>(data)->volatility * 2.5 + data->getCommonFactor();
}

double OptionPricer::calculatePriceFat(const Data* data) const {
    return data->getVolatility() * 2.5 + data->getCommonFactor();
}

int main() {
    constexpr size_t iterations = 10'000;
    std::vector<std::unique_ptr<Data>> dataSamples;
    std::vector<std::unique_ptr<Pricer>> pricers;

    StockPricer stockPricer;
    OptionPricer optionPricer;

    // Generate sample data
    for (size_t i = 0; i < iterations / 2; ++i) {
        dataSamples.emplace_back(std::make_unique<StockData>(1.2, &stockPricer, &stockPricer));
        dataSamples.emplace_back(std::make_unique<OptionData>(0.8, &optionPricer, &optionPricer));
    }
    
    // Benchmark Design 1: Virtual function in Data
    benchmark("Design 1: Virtual function in Data", [&]() {
        for (const auto& data : dataSamples) {
            data->calculatePrice();
        }
    }, iterations);

    // Benchmark Design 2: Dynamic cast in Pricer
    benchmark("Design 2: Dynamic cast in Pricer", [&]() {
        for (const auto& data : dataSamples) {
            data->calculatePriceBaseDynamic();
        }
    }, iterations);

    // Benchmark Design 3: Static cast in Pricer
    benchmark("Design 3: Static cast in Pricer", [&]() {
        for (const auto& data : dataSamples) {
            data->calculatePriceBaseStatic();
        }
    }, iterations);

    // Benchmark Design 4: Calling Derived Pricer inside Derived Data
    benchmark("Design 4: Calling Derived Pricer inside Derived Data", [&]() {
        for (const auto& data : dataSamples) {
            data->calculatePriceDerived();
        }
    }, iterations);

    // Benchmark Design 5: Unified data members in base class
    benchmark("Design 5: Fat data members", [&]() {
        for (const auto& data : dataSamples) {
            data->calculatePriceFat();
        }
    }, iterations);

    return 0;
}