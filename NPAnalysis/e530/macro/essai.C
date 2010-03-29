void essai(TObject *obj, TString name = "dum", TString opt = "U")
{
   // nom du fichier pour l'enregistrement
   if ( !name.Contains(".root") ) name += ".root";

   // option de creation du fichier
   TString option;
   opt.ToUpper();
   if ( opt.Contains("U") ) option = "UPDATE";
   if ( opt.Contains("N") ) option = "NEW";

   // ouverture du fichier et enregistrement de l'objet
   TFile outfile(name, option.Data());
   obj->Write();
   outfile.Close();
}
