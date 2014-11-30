/*---------------------------------------------------------------------------*\
=========                 |
\\      /  F ield         | Unsupported Contributions for OpenFOAM
 \\    /   O peration     |
  \\  /    A nd           | Copyright (C) 2013-2014 OpenFOAM Foundation
   \\/     M anipulation  |
-------------------------------------------------------------------------------
2014-11-30 Bruno Santos (wyldckat):
    Cross-over BC between interstitialInletVelocityFvPatchVectorField and
    uniformFixedValueFvPatchField, but oriented to interpolate data over time.

    Created for fulfilling the request on this thread:
      http://www.cfd-online.com/Forums/openfoam-programming-development/
      121366-time-varying-boundary-condition.html
-------------------------------------------------------------------------------

License
    This file is a derivative work of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    Foam::uniformInterstitialInletVelocityFvPatchVectorField

\*---------------------------------------------------------------------------*/

#include "uniformInterstitialInletVelocityFvPatchVectorField.H"
#include "volFields.H"
#include "addToRunTimeSelectionTable.H"
#include "fvPatchFieldMapper.H"
#include "surfaceFields.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::uniformInterstitialInletVelocityFvPatchVectorField::
uniformInterstitialInletVelocityFvPatchVectorField
(
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF
)
:
    fixedValueFvPatchVectorField(p, iF),
    uniformInletVelocity_(),
    inletVelocity_(p.size(), vector::zero),
    alphaName_("alpha")
{}


Foam::uniformInterstitialInletVelocityFvPatchVectorField::
uniformInterstitialInletVelocityFvPatchVectorField
(
    const uniformInterstitialInletVelocityFvPatchVectorField& ptf,
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fixedValueFvPatchVectorField(ptf, p, iF, mapper),
    uniformInletVelocity_(ptf.uniformInletVelocity_().clone().ptr()),
    inletVelocity_(ptf.inletVelocity_, mapper),
    alphaName_(ptf.alphaName_)
{}


Foam::uniformInterstitialInletVelocityFvPatchVectorField::
uniformInterstitialInletVelocityFvPatchVectorField
(
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF,
    const dictionary& dict
)
:
    fixedValueFvPatchVectorField(p, iF, dict),
    uniformInletVelocity_(DataEntry<vector>::New("uniformInletVelocity", dict)),
    inletVelocity_("inletVelocity", dict, p.size()),
    alphaName_(dict.lookupOrDefault<word>("alpha", "alpha"))
{}


Foam::uniformInterstitialInletVelocityFvPatchVectorField::
uniformInterstitialInletVelocityFvPatchVectorField
(
    const uniformInterstitialInletVelocityFvPatchVectorField& ptf
)
:
    fixedValueFvPatchVectorField(ptf),
    uniformInletVelocity_
    (
        ptf.uniformInletVelocity_.valid()
      ? ptf.uniformInletVelocity_().clone().ptr()
      : NULL
    ),
    inletVelocity_(ptf.inletVelocity_),
    alphaName_(ptf.alphaName_)
{}


Foam::uniformInterstitialInletVelocityFvPatchVectorField::
uniformInterstitialInletVelocityFvPatchVectorField
(
    const uniformInterstitialInletVelocityFvPatchVectorField& ptf,
    const DimensionedField<vector, volMesh>& iF
)
:
    fixedValueFvPatchVectorField(ptf, iF),
    uniformInletVelocity_
    (
        ptf.uniformInletVelocity_.valid()
      ? ptf.uniformInletVelocity_().clone().ptr()
      : NULL
    ),
    inletVelocity_(ptf.inletVelocity_),
    alphaName_(ptf.alphaName_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::uniformInterstitialInletVelocityFvPatchVectorField::autoMap
(
    const fvPatchFieldMapper& m
)
{
    fixedValueFvPatchVectorField::autoMap(m);
    inletVelocity_.autoMap(m);
}


void Foam::uniformInterstitialInletVelocityFvPatchVectorField::rmap
(
    const fvPatchVectorField& ptf,
    const labelList& addr
)
{
    fixedValueFvPatchVectorField::rmap(ptf, addr);

    const uniformInterstitialInletVelocityFvPatchVectorField& tiptf =
        refCast<const uniformInterstitialInletVelocityFvPatchVectorField>(ptf);

    inletVelocity_.rmap(tiptf.inletVelocity_, addr);
}


void Foam::uniformInterstitialInletVelocityFvPatchVectorField::updateCoeffs()
{
    if (updated())
    {
        return;
    }

    const fvPatchField<scalar>& alphap =
        patch().lookupPatchField<volScalarField, scalar>(alphaName_);
    const scalar t = this->db().time().timeOutputValue();
    
    inletVelocity_ = uniformInletVelocity_->value(t);
    operator==(inletVelocity_/alphap);
    fixedValueFvPatchVectorField::updateCoeffs();
}


void Foam::uniformInterstitialInletVelocityFvPatchVectorField::write
(
    Ostream& os
) const
{
    fvPatchField<vector>::write(os);
    writeEntryIfDifferent<word>(os, "alpha", "alpha", alphaName_);
    uniformInletVelocity_->writeData(os);
    inletVelocity_.writeEntry("inletVelocity", os);
    writeEntry("value", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
   makePatchTypeField
   (
       fvPatchVectorField,
       uniformInterstitialInletVelocityFvPatchVectorField
   );
}


// ************************************************************************* //
