void Mask(){

  double dimension = 100.42;
  double active = 97.30;
  double pitch = 97.30/128;
  double width = 0.7;
//
//  double dimension = 100;
//  double active = 97;
//  double pitch = 97/97;
//  double width = 0.5;

  // mm per pixel 
  double scale = 0.005;
  //pitch in pixel
  unsigned int spitch = pitch/scale;
  unsigned int swidth  = width/scale;
  unsigned int sinter = (spitch - swidth)/2;
  cout << spitch << " " << swidth << " " << sinter << endl;
  // image size
  unsigned int size = dimension/scale;
  cout << "Image size: " << size << "x" << size<< endl ;
  double* zargb = new double[size*size];
  TASImage* zero = new TASImage("zero",zargb,size,size,0);
  zero->WriteImage("mask.png");
  TASImage* mask = new TASImage("mask.png");
  delete[] zargb;
  unsigned int* argb = mask->GetArgbArray();
  unsigned int* argb2 = mask->GetArgbArray();
  unsigned int index = 0;
  double border1 = (dimension-active)/scale;
  double border2 = (active)/scale;

  for(unsigned int px = 0 ; px < size ; px++){
    for(unsigned int py = 0 ; py < size ; py++){
      if(px%1000==0)
        cout << "\r" << px << "/" << size << flush; 
      // Compute array index
      index = px * size + py; 
      // Inactive sides
      if(px < border1|| py < border1 || px>border2 || py>border2)
        argb[index] = 0xffff0000;
      else{ // strips
        unsigned int coord = px-border1;
        unsigned int nbr = coord/spitch;
       // cout << coord << " " << nbr*spitch+sinter << " " << spitch << " " << sinter << " " << spitch << " " << sinter << endl;
        if(coord<(nbr*spitch+sinter) || coord >(((nbr+1)*spitch-sinter))){// interstrip
          argb[index] = 0xffff0000+(((nbr+1))<<8)+nbr; 
        }   
        else
          argb[index] = 0xff000000 + nbr;
      }
    }
  }
  mask->WriteImage("mask.png");
  //mask->Draw();

}
