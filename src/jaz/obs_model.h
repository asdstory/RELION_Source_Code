/***************************************************************************
 *
 * Author: "Jasenko Zivanov"
 * MRC Laboratory of Molecular Biology
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * This complete copyright notice must be included in any revised version of the
 * source code. Additional authorship citations may be added, but existing
 * author citations must be preserved.
 ***************************************************************************/

#ifndef OBS_MODEL_H
#define OBS_MODEL_H

#include <src/image.h>
#include <src/fftw.h>
#include <src/complex.h>
#include <src/metadata_table.h>
#include <src/projector.h>
#include <src/jaz/gravis/t2Matrix.h>

class BackProjector;

class ObservationModel
{
    public:

		static void loadSafely(
				std::string filename,
				ObservationModel& obsModel,
				MetaDataTable& particlesMdt, MetaDataTable& opticsMdt, int verb = 0);

		static void save(
				MetaDataTable& particlesMdt,
				MetaDataTable& opticsMdt,
				std::string filename);

		static bool containsAllColumnsNeededForPrediction(const MetaDataTable& partMdt);


        ObservationModel();
        ObservationModel(const MetaDataTable &opticsMdt);


			MetaDataTable opticsMdt;
			bool hasEvenZernike, hasOddZernike, hasMagMatrices, hasBoxSizes;


	protected:
			
			// cached values - protected to prevent users from accidentally changing them,
			// expecting the changes to propagate into the optics star-file
			std::vector<double> angpix, lambda, Cs;
			std::vector<int> boxSizes;
			std::vector<std::vector<double> > evenZernikeCoeffs, oddZernikeCoeffs;
			std::vector<Matrix2D<RFLOAT> > magMatrices;

			// cached aberration effects for a set of given image sizes
			// e.g.: phaseCorr[opt. group][img. height](y,x)
			std::vector<std::map<int,Image<Complex> > > phaseCorr;
			std::vector<std::map<int,Image<RFLOAT> > > gammaOffset;


	public:

	// Prediction //

		void predictObservation(
				Projector &proj, const MetaDataTable &partMdt, long int particle,
				MultidimArray<Complex>& dest, double angpix_ref,
				bool applyCtf = true, bool shiftPhases = true, bool applyShift = true);

		Volume<gravis::t2Vector<Complex> > predictComplexGradient(
				Projector &proj, const MetaDataTable &partMdt, long int particle, double angpix_ref,
				bool applyCtf = true, bool shiftPhases = true, bool applyShift = true);



	// Correction //

		// apply effect of antisymmetric aberration (using cache)
		void demodulatePhase(
				int optGroup,
				MultidimArray<Complex>& obsImage);

		// syntactic sugar
		void demodulatePhase(
				const MetaDataTable &partMdt,
				long int particle,
				MultidimArray<Complex>& obsImage);

		// effect of antisymmetric aberration (cached)
		const Image<Complex>& getPhaseCorrection(int optGroup, int s);

		// effect of symmetric aberration (cached)
		const Image<RFLOAT>& getGammaOffset(int optGroup, int s);

		Matrix2D<RFLOAT> applyAnisoMagTransp(
				Matrix2D<RFLOAT> A3D_transp, int opticsGroup, 
				int s3D = -1, double angpix3D = -1);



	// Bureaucracy //

		// for now, the programs assume that all optics groups have the same pixel size
		bool allPixelSizesIdentical() const;

        double angToPix(double a, int s, int opticsGroup = 0) const;
        double pixToAng(double p, int s, int opticsGroup = 0) const;
		
		double getPixelSize(int opticsGroup) const;
		std::vector<double> getPixelSizes() const;
		
		double getWavelength(int opticsGroup) const;
		std::vector<double> getWavelengths() const;
		
		double getSphericalAberration(int opticsGroup) const;
		std::vector<double> getSphericalAberrations() const;
	
		int getBoxSize(int opticsGroup) const;
		void getBoxSizes(std::vector<int>& sDest, std::vector<int>& shDest) const;

		Matrix2D<RFLOAT> getMagMatrix(int opticsGroup) const;
		std::vector<Matrix2D<RFLOAT> > getMagMatrices() const;
		
		int getOpticsGroup(const MetaDataTable &particlesMdt, int particle) const;

		/* duh */
		int numberOfOpticsGroups() const;

		/* Check whether the optics groups appear in the correct order.
		   This makes it possible to access a group g through:

		       opticsMdt.getValue(label, dest, g-1);
		*/
		bool opticsGroupsSorted() const;

		/* Find all optics groups used in particles table partMdt
		   that are not defined in opticsMdt (should return an empty vector) */
		std::vector<int> findUndefinedOptGroups(const MetaDataTable& partMdt) const;

		/* Rename optics groups to enforce the correct order
		   and translate the indices in particle table partMdt.
		   (Merely changing the order in opticsMdt would fail if groups were missing.) */
		void sortOpticsGroups(MetaDataTable& partMdt);
		
		/* Return the set of optics groups present in partMdt */
		std::vector<int> getOptGroupsPresent_oneBased(const MetaDataTable& partMdt) const;
		
		/* Return the set of optics groups present in partMdt */
		std::vector<int> getOptGroupsPresent_zeroBased(const MetaDataTable& partMdt) const;

		std::vector<std::pair<int, std::vector<int> > > splitParticlesByOpticsGroup(
				const MetaDataTable& partMdt) const;


};

#endif
