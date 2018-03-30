#ifndef JAZ_OPTIMIZATION_H
#define JAZ_OPTIMIZATION_H

#include <vector>

// abstract class for optimization problems
class Optimization
{
    public:

    virtual double f(const std::vector<double>& x, void* tempStorage) const = 0;
    virtual void* allocateTempStorage() const {return 0;}
    virtual void deallocateTempStorage(void* ts) const {}
};

class DifferentiableOptimization : public Optimization
{
    public:

    virtual void grad(const std::vector<double>& x, std::vector<double>& gradDest, void* tempStorage) const = 0;
};

class RosenbrockBanana : public Optimization
{
    public:

    double f(const std::vector<double>& x, void* tempStorage) const;
};

#endif
