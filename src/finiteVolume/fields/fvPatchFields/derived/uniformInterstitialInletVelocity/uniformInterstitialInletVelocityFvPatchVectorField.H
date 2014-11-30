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

Description
    Time dependent inlet velocity in which the actual interstitial velocity is
    calculated by dividing the specified inletVelocity field with the local
    phase-fraction.

    \heading Patch usage

    \table
        Property             | Description             | Required    |
        uniformInletVelocity | Non-interstitial inlet velocity | yes |
        alpha                | Name of the phase-fraction field | yes |
        inletVelocity        | Current non-interstitial inlet velocity | yes |
    \endtable

    Example of the boundary condition specification:
    \verbatim
    inlet
    {
        type                     uniformInterstitialInletVelocity;
        uniformInletVelocity     constant (0 0.2 0);
        alpha                    alpha.particles;
        inletVelocity            uniform (0 0.2 0);
        value                    uniform (0 0 0);
    }
    \endverbatim

SourceFiles
    uniformInterstitialInletVelocityFvPatchVectorField.C

\*---------------------------------------------------------------------------*/

#ifndef uniformInterstitialInletVelocityFvPatchVectorField_H
#define uniformInterstitialInletVelocityFvPatchVectorField_H

#include "fixedValueFvPatchFields.H"
#include "DataEntry.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
/*---------------------------------------------------------------------------*\
               Class uniformInterstitialInletVelocityFvPatch Declaration
\*---------------------------------------------------------------------------*/

class uniformInterstitialInletVelocityFvPatchVectorField
:
    public fixedValueFvPatchVectorField
{
    // Private data

        //- Time varying inlet velocity
        autoPtr<DataEntry<vector> > uniformInletVelocity_;

        //- Inlet velocity
        vectorField inletVelocity_;

        //- Name of the flux transporting the field
        word alphaName_;


public:

   //- Runtime type information
   TypeName("uniformInterstitialInletVelocity");


   // Constructors

        //- Construct from patch and internal field
        uniformInterstitialInletVelocityFvPatchVectorField
        (
            const fvPatch&,
            const DimensionedField<vector, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        uniformInterstitialInletVelocityFvPatchVectorField
        (
            const fvPatch&,
            const DimensionedField<vector, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given
        //  uniformInterstitialInletVelocityFvPatchVectorField
        //  onto a new patch
        uniformInterstitialInletVelocityFvPatchVectorField
        (
            const uniformInterstitialInletVelocityFvPatchVectorField&,
            const fvPatch&,
            const DimensionedField<vector, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        uniformInterstitialInletVelocityFvPatchVectorField
        (
            const uniformInterstitialInletVelocityFvPatchVectorField&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchVectorField> clone() const
        {
            return tmp<fvPatchVectorField>
            (
                new uniformInterstitialInletVelocityFvPatchVectorField
                    (*this)
            );
        }

        //- Construct as copy setting internal field reference
        uniformInterstitialInletVelocityFvPatchVectorField
        (
            const uniformInterstitialInletVelocityFvPatchVectorField&,
            const DimensionedField<vector, volMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual tmp<fvPatchVectorField> clone
        (
            const DimensionedField<vector, volMesh>& iF
        ) const
        {
            return tmp<fvPatchVectorField>
            (
                new uniformInterstitialInletVelocityFvPatchVectorField
                    (*this, iF)
            );
        }


    // Member functions

        // Mapping functions

            //- Map (and resize as needed) from self given a mapping object
            virtual void autoMap
            (
                const fvPatchFieldMapper&
            );

            //- Reverse map the given fvPatchField onto this fvPatchField
            virtual void rmap
            (
                const fvPatchVectorField&,
                const labelList&
            );


        // Evaluation functions

            //- Update the coefficients associated with the patch field
            virtual void updateCoeffs();

        //- Write
        virtual void write(Ostream&) const;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
