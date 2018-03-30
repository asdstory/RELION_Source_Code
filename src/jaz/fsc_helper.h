#ifndef FSC_HELPER_H
#define FSC_HELPER_H

#include <src/ctf.h>
#include <src/image.h>
#include <vector>
#include <src/jaz/gravis/t2Vector.h>
#include <src/jaz/optimization.h>

class BFactorFit : public Optimization
{
    public:

        BFactorFit(
            const Image<RFLOAT>& tau2,
            const Image<RFLOAT>& weight,
            int frame, int cutoff,
            double Bscale, double Cscale);

        double f(const std::vector<double>& x, void* tempStorage) const;

    private:

        const Image<RFLOAT>& tau2;
        const Image<RFLOAT>& weight;
        int frame, cutoff;
        double Bscale, Cscale;
};

class FscHelper
{
    public:

        static void computeFscTable(
                const std::vector<std::vector<Image<Complex> > >& frames,
                const std::vector<Image<Complex> >& predictions,
                Image<RFLOAT>& table, Image<RFLOAT>& weight);

        static void computeFscRow(
                const MultidimArray<Complex>& data0,
                const MultidimArray<Complex>& data1,
                int row, Image<RFLOAT>& table, Image<RFLOAT>& weight);


        static void initFscTable(
                int kc, int tc,
                Image<RFLOAT>& table,
                Image<RFLOAT>& weight0,
                Image<RFLOAT>& weight1);

        static void updateFscTable(
                const std::vector<std::vector<Image<Complex> > >& frames,
                const std::vector<Image<Complex> >& predictions,
                Image<RFLOAT>& table, Image<RFLOAT>& weight0, Image<RFLOAT>& weight1);

        static void updateFscTable(
                const std::vector<Image<Complex> >& frames,
                const Image<Complex>& predictions,
                Image<RFLOAT>& table,
                Image<RFLOAT>& weight0,
                Image<RFLOAT>& weight1);

        static void updateFscTable(
                const Image<Complex>& frame,
                int f,
                const Image<Complex>& prediction,
                Image<RFLOAT>& table,
                Image<RFLOAT>& weight0,
                Image<RFLOAT>& weight1);

        static void updateFscTable(
                const Image<Complex>& frame,
                const Image<RFLOAT>& ctf,
                int f,
                const Image<Complex>& prediction,
                Image<RFLOAT>& table,
                Image<RFLOAT>& weight0,
                Image<RFLOAT>& weight1);

        static void updateFscTableVelWgh(
                const std::vector<Image<Complex> > &frames,
                const std::vector<gravis::d2Vector> &velocities,
                const Image<Complex> &prediction,
                Image<RFLOAT> &table,
                Image<RFLOAT> &weight0,
                Image<RFLOAT> &weight1);


        static void updateVelFscTable(
                const std::vector<Image<Complex> >& frames,
                const std::vector<gravis::d2Vector>& velocities,
                const Image<Complex>& prediction,
                Image<RFLOAT>& table,
                Image<RFLOAT>& weight0,
                Image<RFLOAT>& weight1,
                int kmin = 0, int kmax = -1);

        static void mergeFscTables(
                const std::vector<Image<RFLOAT>>& tables0,
                const std::vector<Image<RFLOAT>>& weights0,
                const std::vector<Image<RFLOAT>>& weights1,
                Image<RFLOAT>& table, Image<RFLOAT>& weight);

        static double computeTsc(
                const std::vector<Image<RFLOAT>>& tables,
                const std::vector<Image<RFLOAT>>& weights0,
                const std::vector<Image<RFLOAT>>& weights1,
                int k0, int k1);

        static void computeNoiseSq(
                std::vector<std::vector<Image<Complex> > > frames,
                std::vector<Image<Complex> > predictions,
                Image<RFLOAT>& sigma2);

        static Image<RFLOAT> computeSignalSq(
                const Image<RFLOAT>& sigma2,
                const Image<RFLOAT>& frc);

        static std::vector<gravis::d2Vector> fitBfactorsNM(
                const Image<RFLOAT>& tau2,
                const Image<RFLOAT>& weight,
                int cutoff);

        static std::vector<gravis::d2Vector> fitBfactors(
                const Image<RFLOAT>& table,
                const Image<RFLOAT>& weight);

        static Image<RFLOAT> tauRatio(
                const Image<RFLOAT>& table,
                const Image<RFLOAT>& weight);

        static void computeBfactors(
                const std::vector<gravis::d2Vector>& bfacs,
                Image<RFLOAT>& table);

        static std::vector<double> powerSpectrum3D(const Image<Complex>& img);
};


#endif
