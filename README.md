# mango

Extracts precursor masses from chimeric spectra of cleavable cross-linkers. Outputs a .ms2 file containing all spectra that precursor 
pairs were detected for, with each spectrum annotated with two precursor masses for each mass relationship found in that spectrum.

Now includes 64bit windows binaries and linux binaries for mango/hardklor. 

Put mango.exe, hardklor, ISOTOPE.dat, hardklor.dat into one directory, and it should run.

Compiling instructions:
Navigate to /mango-master/ or /mango/ (whatever directory contains the Makefile and Hardklor/mstoolkit directories) depending on how you 
grabbed the repo and run Make. Depending on the environment you are running, you might have to compile hardklor separately. 

Install notes:
There are now compiled binaries, so just drop mango.exe, hardklor, ISOTOPE.DAT, hardklor.dat into a directory with MZXML files
and it should be good to go. For linux systems, you still have to add Hardklor to your path environment or it will error out.
For Windows it works as is.

More comprehensive install instructions:
After compiling, add the hardklor executable to your path environment as it's called directly by mango. Additionally, there are two
lines in mango_ProcessManager.cpp (Lines 700,701) that specify the path of the hardklor .dat files used in the analysis. Those two lines
need to be updated with the full path to where those files will be stored on your system (e.g. /usr/etc/hardklor.dat). After updating
those lines in that file, it is necessary to recompile (run make clean followed by make) to update their path in the .conf file. In the
current version it looks in the active directory, so you'll have to have copies there.


Quick usage steps:
1. Run mango.exe -p to generate mango.params.new. It must be renamed to mango.params for it to be used.
2. Change params file to match acquistion method/cross-linker used.
3. Run mango.exe on an mzXML, it will return a .hk1, .hk2, .peaks, and .ms2/.mgf (depending on export parameter). The .hk1/.hk2 files
are hardklor's output for ms1/ms2 scans. The .peaks file lists the masses that fulfilled the mass relationship. The .ms2 contains the
extracted precursor masses with their peaklist. The .ms2 is supported natively with Comet 2017 onwards, just add mango_search=1 to the
comet.params file. If using a different search tool, use Mango's .mgf output, which stores the details to pair spectra in the spectrum
title.
4. Search data using comet or whatever other tool (use mango_search=1 in the comet.params if using comet).
5. Pair results using spectrum title identifier. 

Other notes:

-Mango won't rerun hardklor if the appropriate .hk files are already in the directory. Hardklor is the majority of the runtime, so this
makes it quick to rerun mango on the same set of files if some parameters are initially incorrect. Consequently, if you change the 
hardklor passthrough parameters, you have to get rid of those .hk files first, or mango won't rerun hardklor. 

Parameter notes:

MANGO PARAMS

-mass_tolerance_relationship: Determines what tolerance Mango will call a pair of masses as fulfilling the mass relationship. 

-mass_tolerance_peptide: Determines the tolerance at which Mango will consider a pair of masses as different in the relationship. It 
cuts down on redundancy by not having the same mass pair at slightly offset ppms reported many times in the same scan.

-reporter_neutral_mass: Constant mass offset used to identify precursor masses. This is dependent entirely on the cross-linker used.
The default mass is for BDP-NHP, and a derivation example for DSSO is provided in the supplement, that could be readily extended to
any cross-linkers that have assymetric fragmentation (DSBU,DSSO, etc).

-export_mgf: Writes an .ms2 file if this value is 0, writes a .mgf file if it is 1. The .mgf files are highly redundant and can
be very large depending on how many relationships are found, as each potential precursor is written to an individual scan. Depending
on the search software used, it might be necessary to break the .mgf into a few separate files, as some software struggles to read in
very large files.

-isotope_offset: Determines how many C13 peaks to consider in the relationship to account for errors in monoisotopic peak calling. 
Considers -n to +n peaks for the relationship (2 or 3 will capture most true relationships with Hardklor). 

HARDKLOR PARAMS 

instrument1/2: Mass analyzer used for the ms1/ms2 scans. Usually Orbitrap or FTICR.

resolution1/2: This is the resolution at 400 m/z. Depending on the type of instrument, the resolution specified in the method file refers to either resolution at 200 or 400 m/z. For most orbitrap instruments it is the value at 200 m/z, so divide the method resolution by 1.414 to convert it to the resolution at 400 m/z. FTICRs report their resolution at 400 m/z, so they can be used without modification.

centroid1/2: This is a boolean indicating whether or not the input spectra are centroided or not.

There are other hardklor params, but they are all hardcoded. You can see what they are the .conf1 and .conf2 files returned by Mango.
Note that changing these values in those .conf files won't do change the parameters used to run Hardklor, as they are overwritten
whenever Mango runs Hardklor. The values used should work fairly well for the majority of experiments.

Additional details on hardklor are available from: https://proteome.gs.washington.edu/software/hardklor/

Release notes:

061621 Update: Not a software update, but Mango/Comet/Xlinkprophet is fully compatible with Bruker TimsTOF data. We had good results using
the tdf2mzml converter (https://github.com/mafreitas/tdf2mzml) to generate the mzML. Use instrument = TOF and resolution = 35000 for hardklor
in the mango.params.

021919 Update: There is a typo in figure S4 of the manuscript for the DSSO derivation. "Long arm modification: 85.94264" is incorrect,
it should be "Long arm modification: 85.98264" instead. For DSSO searches you should use 85.98264 for the lysine mod in searches.
The reporter masses in the derivation are correct still. I have also added some DSSO params files for mango+comet, they have to be
renamed to mango.params and comet.params to work as usual. They have the correct masses for DSSO for mango and comet, the rest of the
params you should change as appropriate for your experiment.

082118 Update: Xlinkprophet (https://github.com/brucelab/xlinkprophet) is now officially released and is compatible with the Mango/Comet
pipeline described in the manuscript and here. Instructions for running xlinkprophet on output from Mango/Comet and a test case are
included in that repository. Xlinkprophet takes .pep.xmls as an input. This is objectively better than the Rscript in post-processing.

072618 Update: Added some auxilary software to complete a pipeline from mzXML to fdr filtered cross-links. 
Briefly, I've included comet binaries for a modified version of Comet that writes spectrum titles to its .txt output (output_txtfile=1), 
as well as an R script that will process a directory of comet .txt output files and return paired xlinks at some estimated fdr. It uses 
the tcltk library, so that needs to be installed to run it. Essentially, given an mzXML file, one can run mango file.mzXML, comet 
file.ms2, then run the R script in the directory where comet's output is. A test case is now included in TestCase/ which includes a 
subset of ~30 scans from an E.coli xlinking run so one can check that everything is working correctly on their system.
