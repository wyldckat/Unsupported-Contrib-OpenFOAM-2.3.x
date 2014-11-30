Unsupported Contributions Repository
====================================

[contrib]: http://www.openfoam.org/contrib/unsupported.php
For instructions on using and contributing to the Unsupported Contributions 
Repository, please go to the [`Unsupported Contributions 
Repository in the OpenFOAM Foundation website`][contrib].


2014-11-30 Bruno Santos (wyldckat)
==================================

Changes made:

  * Added the library `src/finiteVolume`, which builds onto the user's binary
    library folder with the name `libcontribFiniteVolume.so`. This library has
    a new boundary condition named `uniformInterstitialInletVelocity`, which
    is Cross-over BC between `interstitialInletVelocityFvPatchVectorField` and
    `uniformFixedValueFvPatchField`.
    
  * Added two tutorial cases in the folder `tutorials/lagrangian/DPMFoam`:
  
    1. `Goldschmidt_uniformInterstitialInletVelocity_230`
       It's the respective `Goldschmidt` tutorial from OpenFOAM 2.3.0, adapted
       to use the new boundary condition.

    2. `Goldschmidt_uniformInterstitialInletVelocity_23x`
       It's the respective `Goldschmidt` tutorial from OpenFOAM 2.3.x, commit
       `c5944c5390`, also adapted to use the new boundary condition.

How to build:
```
wmake libso src/finiteVolume
```

How to run the tutorials:
```
cd tutorials/lagrangian/DPMFoam

#For OpenFOAM 2.3.0
cd Goldschmidt_uniformInterstitialInletVelocity_230

#For OpenFOAM 2.3.x
cd Goldschmidt_uniformInterstitialInletVelocity_23x

#then run the following command
foamRunTutorials
```

How to use the new boundary condition? Have a look at the file `0/U.air` in
each one of the provided tutorial folder and look for this block of code:
```
    bottom
    {
        type            uniformInterstitialInletVelocity;
        uniformInletVelocity   constant (0 0 1.875);
        inletVelocity   uniform (0 0 1.875);
        value           uniform (0 0 1.875);
        phi             phi.air;
        alpha           alpha.air;
    }
```

then compare to the original boundary condition:

```
    bottom
    {
        type            uniformInterstitialInletVelocity;
        inletVelocity   uniform (0 0 1.875);
        value           uniform (0 0 1.875);
        phi             phi.air;
        alpha           alpha.air;
    }
```

and cross-reference with the details provided at:
  http://www.openfoam.org/version2.1.0/boundary-conditions.php
