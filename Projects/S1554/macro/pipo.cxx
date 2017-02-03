void pipo(){


 double* x = new double[2];
 double* y = new double[2];
 x[0] = 0.027; y[0] = -0.029;
 x[1] = 1.431; y[1] = 1.376;

TGraph* g= new TGraph(2,x,y);
g->Draw("ac");
}
