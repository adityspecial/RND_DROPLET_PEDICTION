/* Sessile.c – 120° sessile drop – NO AUTO-STOP */
#include "axi.h"
#include "navier-stokes/centered.h"
#include "two-phase.h"
#include "contact.h"
#include "tension.h"
#include "reduced.h"

/* Parameters */
#define R       0.005
#define pool_depth 0.0*(2*R)
#define gap    -0.5*(2*R)
#define L0     (4*R)
#define tEnd   0.3

/* Contact angle */
vector h[];
double theta0 = 50;
h.t[left] = contact_angle(theta0*pi/180.);

/* Boundaries */
u.n[right] = neumann(0.);
p[right]   = dirichlet(0.);
pf[right]  = dirichlet(0.);
u.n[left]  = dirichlet(0.);
u.t[left]  = dirichlet(0.);

int main() {
  size(L0);
  init_grid(1 << 8);
  TOLERANCE = 1e-3;
  NITERMAX = 100;
  CFL = 0.25;
  DT = 1e-6;
  rho1 = 998;  mu1 = 0.001;
  rho2 = 1.2;  mu2 = 1.8e-5;
  f.sigma = 0.0652;
  f.height = h;
  G.x = -9.81;
  run();
}

/* Initial drop */
double geometry(double x, double y) {
  return -(sq(x - pool_depth - R - gap) + sq(y) - sq(R));
}

event init(t = 0) {
  if (!restore(file = "restart")) {
    fraction(f, geometry(x,y));
    foreach() u.x[] = u.y[] = 0.;
    boundary({f,u});
  }
}

/* Gravity */
event gravity(i++) {
  face vector av = a;
  foreach_face(x) av.x[] -= 9.81;
}

/* Safe refinement */
event adapt(i++) {
  adapt_wavelet({f,u}, (double[]){1e-3,1e-3,1e-3}, maxlevel=7, minlevel=5);
}

/* Hard dt cap */
event timestep(i++) {
  if (dt > 1e-5) dt = 1e-5;
}

/* OUTPUT EVERY 0.5 ms */
event output(t += 0.0005; t <= tEnd) {
  static int nf = 0;
  char name[80];

  /* Interface */
  sprintf(name, "interface/%04d.dat", nf);
  FILE *fp = fopen(name, "w");
  output_facets(f, fp);
  fclose(fp);

  /* VOF + Vorticity */
  scalar vort[], m[];
  vorticity(u, vort);
  foreach() m[] = (f[] > 0.05 && f[] < 0.95);
  boundary({m});

  sprintf(name, "image/vof-%04d.png", nf);
  output_ppm(f, file=name, n=800, min=0, max=1);

  sprintf(name, "image/vort-%04d.png", nf);
  output_ppm(vort, file=name, n=800, min=-5e4, max=5e4,
             map=cool_warm, mask=m);

  /* Dump */
  sprintf(name, "dump/dump-%g", t);
  dump(file=name);

  nf++;
  fprintf(stderr, "t=%.6f  frame=%04d  |u|=%g\n", t, nf-1,
          normf(u.x).max + normf(u.y).max);
}

/* MOVIES */
event movie(t += 0.0001; t <= tEnd) {
  output_ppm(f, file="f.mp4", n=600, min=0, max=1);
}

/* LOG */
event log(i += 10) {
  double umax = 0.;
  foreach(reduction(max:umax))
    umax = max(umax, sqrt(sq(u.x[])+sq(u.y[])));
  fprintf(stderr, "i=%d  t=%g  dt=%g  |u|_max=%g\n", i, t, dt, umax);
}
