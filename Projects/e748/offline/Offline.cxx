/*****************************************************************************
 * Original Author: V. Girard-Alcindor                                       *
 * contact address: girardalcindor@ganil.fr                                   *
 *                                                                           *
 * Creation Date  : June 2017                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 * This programm convert a GANIL run into a root file readable by NpTool     *          
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 * If you want to convert multiple run you should use the run_analysis.sh    *
 * script you will find in this folder                                         *
 *                                                                           *
 *****************************************************************************/

#include <iostream>
#include "GTape.h"
#include "GUser.h"
#include "General.h"
using namespace std;

int main(int argc, char * argv[]){

  std::string run, output_run;
  if(argc!=1){
    run=argv[1]; // Run name
    output_run=argv[2]; // Output run.root
  }

  cout << " * Looking for Run " << run << endl ;

    GTape *file = new GTape(run.c_str());
    /// analyse
    /// Open
    file->Open();
    GUser * a= new GUser(file); // creat user treatement environement
    //GNetServerRoot *serv = new GNetServerRoot(9094, a);
    //a->EventInit("must2"); // even initialisation
    a->EventInit("e748"); // even initialisation
    /// set tree mode and name
    //a->SetSpectraMode(1); // Declare all raw parameters as histograms
    a->SetSpectraMode(0); 

    a->SetTTreeMode((TTreeMode)3, output_run.c_str());
    /// work the data
    a->InitUser();
    //serv->StartServer();
    file->Rewind();
    a->DoRun(); 
    //a->DoRun(2000); //do treaments on 2000 first events ( 0 = all);
    /// wrap-up and save spectra with a specific name
    a->EndUser(); // must be explicitly called , if it needs
    /* a->SpeSave(histName); // save all declared histogram */
    // delete Guser object
    delete (a); // finish

    return 0;
}


