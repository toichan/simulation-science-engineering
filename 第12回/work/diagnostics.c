# include <stdio.h>
# include <stdlib.h>

FILE *gpid, *fp;

void init_diagnostics (int file, char* filename,
		       double lx, double umin, double umax) {

  if (file) {
    if ( (fp = fopen (filename, "w")) == NULL ) {
      fprintf (stderr, "ERROR: cannot open file.\n");
      exit (EXIT_FAILURE);
    }
  } else {
    if ( (gpid = popen ("gnuplot -persist", "w")) == NULL ) {
      fprintf (stderr, "ERROR: gnuplot not found.\n");
      exit (EXIT_FAILURE);
    }
    fprintf (gpid, "set grid\n");
    fprintf (gpid, "set xlabel 'x'\n");
    fprintf (gpid, "set xrange [0:%g]\n", lx);
    //fprintf (gpid, "set xrange [%g:%g]\n", -lx, lx);
    fprintf (gpid, "set yrange [%g:%g]\n", umin, umax);
    //fprintf (gpid, "set yrange [-10:10]\n");
  }

}

void finish_diagnostics (int file) {
  if (file) fclose (fp);
  else pclose (gpid);
}

void loop_diagnostics (int file, double time, const double *xx,
		       const double *ff, int nx) {
  int ix;
  if (file) {

    for (ix=0; ix<=nx; ix++)
      fprintf (fp, "time= %g, xx= %g, unew= %g\n", time, xx[ix], ff[ix]);
    fprintf (fp, "\n"); // データの区切りとして最後に改行を入れる
    // もし unew の積分値を出力したいなら、ここで数値積分して、画面なり、
    // ファイルなりに出力 ***************************************************

  } else {

    fprintf (gpid, "plot '-' title 'time=%g' with linespoints\n", time);
    for (ix=0; ix<=nx; ix++) fprintf (gpid, "%g %g\n", xx[ix], ff[ix]);
    fprintf (gpid, "end\n");
    fprintf (gpid, "pause 0.05\n");
    fflush (gpid);

  }
}
