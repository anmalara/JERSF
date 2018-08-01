#ifndef MYJET_H_
#define MYJET_H_

#include <TLorentzVector.h>

class MyJet : public TLorentzVector
{

	public:

		MyJet();
		MyJet(double pt, double eta, double phi, double m )
		{
			SetPtEtaPhiE( pt, eta, phi, m );
		};

		virtual ~MyJet();

	private:

};

#endif
