/******************************************************************************
CosmoLike Configuration Space Covariances for Projected Galaxy 2-Point Statistics
https://github.com/CosmoLike/CosmoCov
by CosmoLike developers
******************************************************************************/

void run_cov_shear_shear_real(char *OUTFILE, char *PATH, double *theta, double *dtheta, int Ntheta, int n1, int n2, int pm1, int pm2, int start);
void run_cov_clustering_real(char *OUTFILE, char *PATH, double *theta, double *dtheta,int Ntheta, int n1, int n2, int start);
void run_cov_ggl_real(char *OUTFILE, char *PATH, double *theta, double *dtheta,int Ntheta, int n1, int n2, int start);
void run_cov_ggl_shear_real(char *OUTFILE, char *PATH, double *theta, double *dtheta, int Ntheta, int n1, int n2, int pm, int start);
void run_cov_clustering_shear_real(char *OUTFILE, char *PATH, double *theta, double *dtheta,int Ntheta, int n1, int n2, int pm, int start);
void run_cov_clustering_ggl_real(char *OUTFILE, char *PATH, double *theta, double *dtheta,int Ntheta, int n1, int n2, int start);

void run_cov_shear_shear_real_bin(char *OUTFILE, char *PATH, double *theta, double *dtheta, int Ntheta, int n1, int n2, int pm1, int pm2, int start);
void run_cov_ggl_real_bin(char *OUTFILE, char *PATH, double *theta, double *dtheta,int Ntheta, int n1, int n2, int start);
void run_cov_clustering_real_bin(char *OUTFILE, char *PATH, double *theta, double *dtheta, int Ntheta, int n1, int n2, int start);
void run_cov_ggl_shear_real_bin(char *OUTFILE, char *PATH, double *theta, double *dtheta, int Ntheta, int n1, int n2, int pm, int start);
void run_cov_clustering_shear_real_bin(char *OUTFILE, char *PATH, double *theta, double *dtheta, int Ntheta, int n1, int n2, int pm, int start);
void run_cov_clustering_ggl_real_bin(char *OUTFILE, char *PATH, double *theta, double *dtheta, int Ntheta, int n1, int n2, int start);

void print_citations(FILE *F1){
  fprintf(F1, "# Please cite the following papers in research using this covariance:\n");
  fprintf(F1, "# arXiv: 1601.05779, https://arxiv.org/abs/1601.05779\n");
  fprintf(F1, "# arXiv: 1911.11947, https://arxiv.org/abs/1911.11947\n");
  fprintf(F1, "# arXiv: 2004.04833, https://arxiv.org/abs/2004.04833\n");
  if (w_mask(like.vtmin) < 1.0){
    fprintf(F1, "# arXiv: 1804.10663, https://arxiv.org/abs/1804.10663\n");
  }
  fprintf(F1, "########################################################\n");
}

void print_ordering(FILE *F2, int i, double t, char *probe, char *sample_type, int z1, char *sample_type2, int z2){
    fprintf(F2, "%d %e %s %s%d %s%d\n", i, t, probe, sample_type, z1, sample_type2, z2);
}

void run_cov_clustering_real_bin(char *OUTFILE, char *PATH, double *theta, double *dtheta, int Ntheta, int n1, int n2, int start)
{
  int z1,z2,z3,z4,nl1,nl2;
  double c_ng, c_g;
  double fsky = survey.area/41253.0;
  FILE *F1;
  char filename[300];
  sprintf(filename,"%s%s_%d",PATH,OUTFILE,start);
  F1 =fopen(filename,"w");
  if(covparams.full_tomo){
    z1 = Zcl1(n1); z2 = Zcl2(n1);
    z3 = Zcl1(n2); z4 = Zcl2(n2);
    printf("N_cl_1 = %d (%d, %d), N_cl_2 = %d (%d, %d)\n", n1,z1,z2, n2,z3,z4);
  } else{
    z1 = n1; z2 = n1;
    z3 = n2; z4 = n2;
    printf("N_cl_1 = %d, N_cl_2 = %d\n", n1,n2);
  }

  double **cov_fullsky_G = 0, **cov_fullsky_NG = 0;
  cov_fullsky_G = create_double_matrix(0, like.Ntheta-1, 0, like.Ntheta-1);
  cov_fullsky_NG = create_double_matrix(0, like.Ntheta-1, 0, like.Ntheta-1);
  cov_cl_cl_real_binned_fullsky(cov_fullsky_G,cov_fullsky_NG,z1,z2,z3,z4, covparams.ng, theta, dtheta);

  print_citations(F1);

  FILE *F2;
  char filename2[300];
  int if_write = 0, i1;
  i1 = Ntheta*(2*tomo.shear_Npowerspectra+tomo.ggl_Npowerspectra+n1);
  sprintf(filename2,"%sorder_%s_i_%d-%d",PATH,covparams.filename,i1,i1+Ntheta-1);
  if (fopen(filename2, "r") == NULL){
    if_write=1;
    F2 =fopen(filename2,"w");
    fprintf(F2, "# i, bin-averaged theta_i, 2pt func, {s: source, l:lens}tomo bin index 1, {s, l}tomo bin index 2\n");
  }

  for (nl1 = 0; nl1 < Ntheta; nl1 ++){
  	double t1 = 2./3.*(pow(theta[nl1+1],3.)-pow(theta[nl1],3.))/(pow(theta[nl1+1],2.)-pow(theta[nl1],2.));
    for (nl2 = 0; nl2 < Ntheta; nl2 ++){
	  double t2 = 2./3.*(pow(theta[nl2+1],3.)-pow(theta[nl2],3.))/(pow(theta[nl2+1],2.)-pow(theta[nl2],2.));
      c_ng = 0.;
      c_g = cov_fullsky_G[nl1][nl2];
      if (z1 == z3 && covparams.ng){c_ng = cov_fullsky_NG[nl1][nl2];}
      fprintf(F1,"%d %d %e %e %d %d %d %d %e %e\n", Ntheta*(2*tomo.shear_Npowerspectra+tomo.ggl_Npowerspectra+n1)+nl1,Ntheta*(2*tomo.shear_Npowerspectra+tomo.ggl_Npowerspectra + n2)+nl2,t1,t2,z1,z2,z3,z4,c_g,c_ng);
    }
    if(if_write) {
      fprintf(F2, "%d %e w l%d l%d\n", i1+nl1, t1, z1, z2);
    }
  }
  free_double_matrix(cov_fullsky_G,0, like.Ntheta-1, 0, like.Ntheta-1);
  free_double_matrix(cov_fullsky_NG,0, like.Ntheta-1, 0, like.Ntheta-1);
  fclose(F1);
  if(if_write) {fclose(F2);}
}

void run_cov_ggl_real_bin(char *OUTFILE, char *PATH, double *theta, double *dtheta,int Ntheta, int n1, int n2, int start)
{
  int zl1,zl2,zs1,zs2,nl1,nl2;
  double c_ng, c_g;
  double fsky = survey.area/41253.0;
  FILE *F1;
  char filename[300];
  sprintf(filename,"%s%s_%d",PATH,OUTFILE,start);
  F1 =fopen(filename,"w");

  zl1 = ZL(n1); zs1 = ZS(n1);
  printf("\nN_tomo_1 = %d (%d, %d)\n", n1,zl1,zs1);
  zl2 = ZL(n2); zs2 = ZS(n2);
  printf("N_tomo_2 = %d (%d, %d)\n", n2,zl2,zs2);

  double **cov_fullsky_G = 0, **cov_fullsky_NG = 0;
  cov_fullsky_G = create_double_matrix(0, like.Ntheta-1, 0, like.Ntheta-1);
  cov_fullsky_NG = create_double_matrix(0, like.Ntheta-1, 0, like.Ntheta-1);
  cov_gl_gl_real_binned_fullsky(cov_fullsky_G,cov_fullsky_NG,zl1,zs1,zl2,zs2, covparams.ng, theta, dtheta);

  print_citations(F1);

  FILE *F2;
  char filename2[300];
  int if_write = 0, i1;
  i1 = Ntheta*(2*tomo.shear_Npowerspectra+n1);
  sprintf(filename2,"%sorder_%s_i_%d-%d",PATH,covparams.filename,i1,i1+Ntheta-1);
  if (fopen(filename2, "r") == NULL){
    if_write=1;
    F2 =fopen(filename2,"w");
    fprintf(F2, "# i, bin-averaged theta_i, 2pt func, {s: source, l:lens}tomo bin index 1, {s, l}tomo bin index 2\n");
  }

  for (nl1 = 0; nl1 < Ntheta; nl1 ++){
  	double t1 = 2./3.*(pow(theta[nl1+1],3.)-pow(theta[nl1],3.))/(pow(theta[nl1+1],2.)-pow(theta[nl1],2.));
    for (nl2 = 0; nl2 < Ntheta; nl2 ++){
	  double t2 = 2./3.*(pow(theta[nl2+1],3.)-pow(theta[nl2],3.))/(pow(theta[nl2+1],2.)-pow(theta[nl2],2.));
      c_ng = 0.;
      // if(NG && zl1 == zl2 && test_zoverlap(zl1,zs1)*test_zoverlap(zl2,zs2)){c_ng = cov_NG_gl_gl_real_binned(theta[nl1],theta[nl1+1],theta[nl2],theta[nl2+1],zl1,zs1,zl2,zs2);}
      if(covparams.ng && zl1 == zl2 && test_zoverlap(zl1,zs1)*test_zoverlap(zl2,zs2)){c_ng = cov_fullsky_NG[nl1][nl2];}
      c_g = cov_fullsky_G[nl1][nl2];
      fprintf(F1,"%d %d %e %e %d %d %d %d %e %e\n", Ntheta*(2*tomo.shear_Npowerspectra+n1)+nl1,Ntheta*(2*tomo.shear_Npowerspectra+n2)+nl2,t1,t2,zl1,zs1,zl2,zs2,c_g,c_ng);
    }
    if(if_write) {
      fprintf(F2, "%d %e gammat l%d s%d\n", i1+nl1, t1, zl1, zs1);
    }
  }
  free_double_matrix(cov_fullsky_G,0, like.Ntheta-1, 0, like.Ntheta-1);
  free_double_matrix(cov_fullsky_NG,0, like.Ntheta-1, 0, like.Ntheta-1);
  fclose(F1);
  if(if_write) {fclose(F2);}
}
void run_cov_shear_shear_real_bin(char *OUTFILE, char *PATH,double *theta, double *dtheta, int Ntheta, int n1, int n2, int pm1, int pm2, int start)
{
  int z1,z2,z3,z4,nl1,nl2;
  double c_ng, c_g,sn;
  double fsky = survey.area/41253.0;
  FILE *F1;
  char filename[300];
  z1 = Z1(n1); z2 = Z2(n1);
  printf("N_shear1 = %d (%d,%d)\n", n1,z1,z2);
  z3 = Z1(n2); z4 = Z2(n2);
  printf("N_shear2 = %d (%d, %d)\n",n2,z3,z4);
  sprintf(filename,"%s%s_%d",PATH,OUTFILE,start);
  F1 =fopen(filename,"w");


  double **cov_fullsky_G = 0, **cov_fullsky_NG = 0;
  cov_fullsky_G = create_double_matrix(0, like.Ntheta-1, 0, like.Ntheta-1);
  cov_fullsky_NG = create_double_matrix(0, like.Ntheta-1, 0, like.Ntheta-1);
  cov_shear_shear_real_binned_fullsky(cov_fullsky_G,cov_fullsky_NG,z1,z2,z3,z4,pm1,pm2, covparams.ng, theta, dtheta);

  print_citations(F1);

  FILE *F2;
  char filename2[300];
  int if_write = 0, i1;
  if(pm1==0) {i1 = Ntheta*(tomo.shear_Npowerspectra+n1);}
  else {i1 = Ntheta*n1;}
  sprintf(filename2,"%sorder_%s_i_%d-%d",PATH,covparams.filename,i1,i1+Ntheta-1);
  if (fopen(filename2, "r") == NULL){
    if_write=1;
    F2 =fopen(filename2,"w");
    fprintf(F2, "# i, bin-averaged theta_i, 2pt func, {s: source, l:lens}tomo bin index 1, {s, l}tomo bin index 2\n");
  }

  for (nl1 = 0; nl1 < Ntheta; nl1 ++){
  	double t1 = 2./3.*(pow(theta[nl1+1],3.)-pow(theta[nl1],3.))/(pow(theta[nl1+1],2.)-pow(theta[nl1],2.));
    for (nl2 = 0; nl2 < Ntheta; nl2 ++){
	  double t2 = 2./3.*(pow(theta[nl2+1],3.)-pow(theta[nl2],3.))/(pow(theta[nl2+1],2.)-pow(theta[nl2],2.));
      c_ng = 0.; c_g = 0.;sn = 0.;
      if(covparams.ng){ c_ng = cov_fullsky_NG[nl1][nl2];}
      c_g = cov_fullsky_G[nl1][nl2];
      if(pm1==1 && pm2==1) fprintf(F1,"%d %d %e %e %d %d %d %d %e %e\n", Ntheta*n1+nl1,Ntheta*(n2)+nl2, t1,t2,z1,z2,z3,z4,c_g,c_ng);
      if(pm1==0 && pm2==0) fprintf(F1,"%d %d %e %e %d %d %d %d %e %e\n", Ntheta*(tomo.shear_Npowerspectra+n1)+nl1,Ntheta*(tomo.shear_Npowerspectra+n2)+nl2,t1,t2,z1,z2,z3,z4,c_g,c_ng);
      if(pm1==1 && pm2==0) fprintf(F1,"%d %d %e %e %d %d %d %d %e %e\n", Ntheta*n1+nl1,Ntheta*(tomo.shear_Npowerspectra+n2)+nl2,t1,t2,z1,z2,z3,z4,c_g,c_ng);
    }
    if(if_write) {
      if(pm1==0){
        fprintf(F2, "%d %e xi- s%d s%d\n", i1+nl1, t1, z1, z2);
      }
      else{
        fprintf(F2, "%d %e xi+ s%d s%d\n", i1+nl1, t1, z1, z2);
      }
    }
  }
  free_double_matrix(cov_fullsky_G,0, like.Ntheta-1, 0, like.Ntheta-1);
  free_double_matrix(cov_fullsky_NG,0, like.Ntheta-1, 0, like.Ntheta-1);
  fclose(F1);
  if(if_write) {fclose(F2);}
}

void run_cov_ggl_shear_real_bin(char *OUTFILE, char *PATH, double *theta, double *dtheta, int Ntheta, int n1, int n2, int pm, int start)
{
  int zl,zs,z3,z4,nl1,nl2;
  double c_ng, c_g;
  double fsky = survey.area/41253.0;
  FILE *F1;
  char filename[300];
  sprintf(filename,"%s%s_%d",PATH,OUTFILE,start);
  F1 =fopen(filename,"w");
  zl = ZL(n1); zs = ZS(n1);
  printf("\nN_ggl = %d (%d, %d)\n", n1,zl,zs);
  z3 = Z1(n2); z4 = Z2(n2);
  printf("N_shear = %d (%d, %d)\n", n2,z3,z4);

  double **cov_fullsky_G = 0, **cov_fullsky_NG = 0;
  cov_fullsky_G = create_double_matrix(0, like.Ntheta-1, 0, like.Ntheta-1);
  cov_fullsky_NG = create_double_matrix(0, like.Ntheta-1, 0, like.Ntheta-1);
  cov_gl_shear_real_binned_fullsky(cov_fullsky_G,cov_fullsky_NG,zl,zs,z3,z4,pm, covparams.ng, theta, dtheta);

  print_citations(F1);

  FILE *F2;
  char filename2[300];
  int if_write = 0, i1;
  i1 = Ntheta*(2*tomo.shear_Npowerspectra+n1);
  sprintf(filename2,"%sorder_%s_i_%d-%d",PATH,covparams.filename,i1,i1+Ntheta-1);
  if (fopen(filename2, "r") == NULL){
    if_write=1;
    F2 =fopen(filename2,"w");
    fprintf(F2, "# i, bin-averaged theta_i, 2pt func, {s: source, l:lens}tomo bin index 1, {s, l}tomo bin index 2\n");
  }

  for (nl1 = 0; nl1 < Ntheta; nl1 ++){
  	double t1 = 2./3.*(pow(theta[nl1+1],3.)-pow(theta[nl1],3.))/(pow(theta[nl1+1],2.)-pow(theta[nl1],2.));
    for (nl2 = 0; nl2 < Ntheta; nl2 ++){
	  double t2 = 2./3.*(pow(theta[nl2+1],3.)-pow(theta[nl2],3.))/(pow(theta[nl2+1],2.)-pow(theta[nl2],2.));
      c_ng = 0.; c_g = 0.;
      // if (test_zoverlap(zl,zs)*test_zoverlap(zl,z3)*test_zoverlap(zl,z4) && NG){ c_ng = cov_NG_gl_shear_real_binned(theta[nl1],theta[nl1+1],theta[nl2],theta[nl2+1],zl,zs,z3,z4,pm);}
      if (test_zoverlap(zl,zs)*test_zoverlap(zl,z3)*test_zoverlap(zl,z4) && covparams.ng){ c_ng = cov_fullsky_NG[nl1][nl2];}
      c_g = cov_fullsky_G[nl1][nl2];
      if(pm==1) fprintf(F1, "%d %d %e %e %d %d %d %d %e %e\n", Ntheta*(2*tomo.shear_Npowerspectra+n1)+nl1,Ntheta*(n2)+nl2,t1,t2,zl,zs,z3,z4,c_g,c_ng);
      if(pm==0) fprintf(F1, "%d %d %e %e %d %d %d %d %e %e\n", Ntheta*(2*tomo.shear_Npowerspectra+n1)+nl1,Ntheta*(tomo.shear_Npowerspectra+n2)+nl2,t1,t2,zl,zs,z3,z4,c_g,c_ng);
    }
    if(if_write) {
      fprintf(F2, "%d %e gammat l%d s%d\n", i1+nl1, t1, zl, zs);
    }
  }
  free_double_matrix(cov_fullsky_G,0, like.Ntheta-1, 0, like.Ntheta-1);
  free_double_matrix(cov_fullsky_NG,0, like.Ntheta-1, 0, like.Ntheta-1);
  fclose(F1);
  if(if_write) {fclose(F2);}
}
void run_cov_clustering_shear_real_bin(char *OUTFILE, char *PATH, double *theta, double *dtheta, int Ntheta, int n1, int n2, int pm, int start)
{
  int z1,z2,z3,z4,nl1,nl2;
  double c_ng, c_g;
  double fsky = survey.area/41253.0;
  FILE *F1;
  char filename[300];
  sprintf(filename,"%s%s_%d",PATH,OUTFILE,start);
  F1 =fopen(filename,"w");
  if(covparams.full_tomo){
    z1 = Zcl1(n1); z2 = Zcl2(n1);
    printf("\nN_cl = %d (%d, %d) \n", n1,z1,z2);
  } else{
    z1 = n1; z2 = n1;
    printf("\nN_cl = %d \n", n1);
  }
  z3 = Z1(n2); z4 = Z2(n2);
  printf("N_shear = %d (%d, %d)\n", n2,z3,z4);

  double **cov_fullsky_G = 0, **cov_fullsky_NG = 0;
  cov_fullsky_G = create_double_matrix(0, like.Ntheta-1, 0, like.Ntheta-1);
  cov_fullsky_NG = create_double_matrix(0, like.Ntheta-1, 0, like.Ntheta-1);
  cov_cl_shear_real_binned_fullsky(cov_fullsky_G,cov_fullsky_NG,z1,z2,z3,z4,pm, covparams.ng, theta, dtheta);

  print_citations(F1);

  FILE *F2;
  char filename2[300];
  int if_write = 0, i1;
  i1 = Ntheta*(2*tomo.shear_Npowerspectra+tomo.ggl_Npowerspectra+n1);
  sprintf(filename2,"%sorder_%s_i_%d-%d",PATH,covparams.filename,i1,i1+Ntheta-1);
  if (fopen(filename2, "r") == NULL){
    if_write=1;
    F2 =fopen(filename2,"w");
    fprintf(F2, "# i, bin-averaged theta_i, 2pt func, {s: source, l:lens}tomo bin index 1, {s, l}tomo bin index 2\n");
  }

  for (nl1 = 0; nl1 < Ntheta; nl1 ++){
  	double t1 = 2./3.*(pow(theta[nl1+1],3.)-pow(theta[nl1],3.))/(pow(theta[nl1+1],2.)-pow(theta[nl1],2.));
    for (nl2 = 0; nl2 < Ntheta; nl2 ++){
	  double t2 = 2./3.*(pow(theta[nl2+1],3.)-pow(theta[nl2],3.))/(pow(theta[nl2+1],2.)-pow(theta[nl2],2.));
      c_ng = 0.; c_g = 0.;
      // if (test_zoverlap(z1,z3)*test_zoverlap(z1,z4) && NG){ c_ng = cov_NG_cl_shear_real_binned(theta[nl1],theta[nl1+1],theta[nl2],theta[nl2+1],z1,z2,z3,z4,pm);}
      if (test_zoverlap(z1,z3)*test_zoverlap(z1,z4) && covparams.ng){ c_ng = cov_fullsky_NG[nl1][nl2];}
      c_g = cov_fullsky_G[nl1][nl2];
      if(pm==1)fprintf(F1,"%d %d %e %e %d %d %d %d %e %e\n", Ntheta*(2*tomo.shear_Npowerspectra+tomo.ggl_Npowerspectra+n1)+nl1,Ntheta*(n2)+nl2,t1,t2,z1,z2,z3,z4,c_g,c_ng);
      if(pm==0)fprintf(F1,"%d %d %e %e %d %d %d %d %e %e\n", Ntheta*(2*tomo.shear_Npowerspectra+tomo.ggl_Npowerspectra+n1)+nl1,Ntheta*(tomo.shear_Npowerspectra+n2)+nl2, t1,t2,z1,z2,z3,z4,c_g,c_ng);
    }
    if(if_write) {
      fprintf(F2, "%d %e w l%d l%d\n", i1+nl1, t1, z1, z2);
    }
  }
  free_double_matrix(cov_fullsky_G,0, like.Ntheta-1, 0, like.Ntheta-1);
  free_double_matrix(cov_fullsky_NG,0, like.Ntheta-1, 0, like.Ntheta-1);
  fclose(F1);
  if(if_write) {fclose(F2);}
}

void run_cov_clustering_ggl_real_bin(char *OUTFILE, char *PATH, double *theta, double *dtheta, int Ntheta, int n1, int n2, int start)
{
  int z1,z2,zl,zs,nl1,nl2;
  double c_ng, c_g;
  double fsky = survey.area/41253.0;
  FILE *F1;
  char filename[300];
  sprintf(filename,"%s%s_%d",PATH,OUTFILE,start);
  F1 =fopen(filename,"w");
  if(covparams.full_tomo){
    z1 = Zcl1(n1); z2 = Zcl2(n1);
    printf("\nN_cl = %d (%d, %d) \n", n1,z1,z2);
  } else{
    z1 = n1; z2 = n1;
    printf("\nN_cl = %d \n", n1);
  }
  zl = ZL(n2); zs = ZS(n2);
  printf("N_tomo_2 = %d (%d, %d)\n", n2,zl,zs);

  double **cov_fullsky_G = 0, **cov_fullsky_NG = 0;
  cov_fullsky_G = create_double_matrix(0, like.Ntheta-1, 0, like.Ntheta-1);
  cov_fullsky_NG = create_double_matrix(0, like.Ntheta-1, 0, like.Ntheta-1);
  cov_cl_gl_real_binned_fullsky(cov_fullsky_G,cov_fullsky_NG,z1,z2,zl,zs, covparams.ng, theta, dtheta);

  print_citations(F1);

  FILE *F2;
  char filename2[300];
  int if_write = 0, i1;
  i1 = Ntheta*(2*tomo.shear_Npowerspectra+tomo.ggl_Npowerspectra+n1);
  sprintf(filename2,"%sorder_%s_i_%d-%d",PATH,covparams.filename,i1,i1+Ntheta-1);
  if (fopen(filename2, "r") == NULL){
    if_write=1;
    F2 =fopen(filename2,"w");
    fprintf(F2, "# i, bin-averaged theta_i, 2pt func, {s: source, l:lens}tomo bin index 1, {s, l}tomo bin index 2\n");
  }

  for (nl1 = 0; nl1 < Ntheta; nl1 ++){
  	double t1 = 2./3.*(pow(theta[nl1+1],3.)-pow(theta[nl1],3.))/(pow(theta[nl1+1],2.)-pow(theta[nl1],2.));
    for (nl2 = 0; nl2 < Ntheta; nl2 ++){
	  double t2 = 2./3.*(pow(theta[nl2+1],3.)-pow(theta[nl2],3.))/(pow(theta[nl2+1],2.)-pow(theta[nl2],2.));
      c_ng = 0.;
      c_g = cov_fullsky_G[nl1][nl2];
      // if (z1 == zl && NG && test_zoverlap(z1,zs)*test_zoverlap(zl,zs)){c_ng = cov_NG_cl_gl_real_binned(theta[nl1],theta[nl1+1],theta[nl2],theta[nl2+1],z1,z2,zl,zs);}
      if (z1 == zl && covparams.ng && test_zoverlap(z1,zs)*test_zoverlap(zl,zs)){c_ng = cov_fullsky_NG[nl1][nl2];}
      fprintf(F1,"%d %d %e %e %d %d %d %d %e %e\n", Ntheta*(2*tomo.shear_Npowerspectra+tomo.ggl_Npowerspectra+n1)+nl1,Ntheta*(2*tomo.shear_Npowerspectra+n2)+nl2,t1,t2,z1,z2,zl,zs,c_g,c_ng);
    }
    if(if_write) {
      fprintf(F2, "%d %e w l%d l%d\n", i1+nl1, t1, z1, z2);
    }
  }
  free_double_matrix(cov_fullsky_G,0, like.Ntheta-1, 0, like.Ntheta-1);
  free_double_matrix(cov_fullsky_NG,0, like.Ntheta-1, 0, like.Ntheta-1);
  fclose(F1);
  if(if_write) {fclose(F2);}
}
