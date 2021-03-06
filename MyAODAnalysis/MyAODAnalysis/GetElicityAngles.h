// Dear emacs, this is -*- c++ -*-
// $Id: GetElicityAngles.h 516537 2012-09-06 16:04:14Z krasznaa $
#ifndef HIGGSZZ4LUTILS_GETELICITYANGLES_H
#define HIGGSZZ4LUTILS_GETELICITYANGLES_H

// ROOT include(s):
#include <Rtypes.h>

// Forward declaration(s):
class TLorentzVector;

/**
 *  @short Function for calculating the relevant angles in a H->ZZ->llll decay
 *
 *         The function calculates the 5 independent angles of the Higgs
 *         4-lepton decay following the definitions in
 *
 *              http://arxiv.org/pdf/1208.4018.pdf
 *
 *         It takes the 4-momenta of the 4 leptons after all corrections
 *         applied, and fills the 5 floating point variables given to it,
 *         in a "C-style".
 *
 * @author Valerio Ippolito <Valerio.Ippolito@cern.ch>
 *
 * @param v1 [input] Negatively charged lepton from the leading Z (Z1 l-)
 * @param v2 [input] Positively charged lepton from the leading Z (Z1 l+)
 * @param v3 [input] Negatively charged lepton from the trailing Z (Z2 l-)
 * @param v4 [input] Positively charged lepton from the trailing Z (Z2 l+)
 * @param cthstr [output] Cosine of theta star (production angle)
 * @param phi1   [output] Phi1 (production angle)
 * @param cth1   [output] Cosine of theta 1 (decay angle)
 * @param cth2   [output] Cosine of theta 2 (decay angle)
 * @param phi    [output] Phi (decay angle)
 *
 * $Revision: 516537 $
 * $Date: 2012-09-06 18:04:14 +0200 (Thu, 06 Sep 2012) $
 */
void GetElicityAngles( const TLorentzVector& v1, const TLorentzVector& v2,
                       const TLorentzVector& v3, const TLorentzVector& v4,
                       Float_t* cthstr, Float_t* phi1,
                       Float_t* cth1, Float_t* cth2, Float_t* phi );

void GetElicityAngles( const TLorentzVector& v1, const TLorentzVector& v2,
                       Float_t* cthstr );

#endif // HIGGSZZ4LUTILS_GETELICITYANGLES_H
