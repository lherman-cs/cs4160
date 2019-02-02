#include "main/gopher.h"
#include "shapes/circle.h"
#include "shapes/container.h"
#include "shapes/ellipse.h"
#include "shapes/line.h"
#include "shapes/rect.h"
#include "shapes/triangle.h"
#define mw 100.5
#define mh 136.03

std::shared_ptr<const Container> buildGopher(int x, int y, int h) {
  int w = h * mw / mh;

  // create ears
  auto leftEar =
      CircleBuilder(w * (3.61 / mw), h * (7.45 / mh), w * (22.12 / mw / 2))
          .setFillMode(Fill::SOLID_OUTLINED)
          .setFillColor(Colors::GOPHER)
          .build();
  auto leftEar1 =
      CircleBuilder(w * (9.38 / mw), h * (13.94 / mh), w * (6.25 / mw / 2))
          .setFillMode(Fill::SOLID)
          .setFillColor(Colors::BLACK)
          .build();

  auto rightEar =
      CircleBuilder(w * (71.41 / mw), h * (6.01 / mh), w * (22.12 / mw / 2))
          .setFillMode(Fill::SOLID_OUTLINED)
          .setFillColor(Colors::GOPHER)
          .build();
  auto rightEar1 =
      CircleBuilder(w * (80.19 / mw), h * (12.02 / mh), w * (6.25 / mw / 2))
          .setFillMode(Fill::SOLID)
          .setFillColor(Colors::BLACK)
          .build();

  // create left eye
  auto leftEye =
      CircleBuilder(w * (21.4 / mw), h * (9.14 / mh), w * (22.12 / mw / 2))
          .setFillMode(Fill::SOLID_OUTLINED)
          .setFillColor(Colors::WHITE)
          .build();
  auto leftEye1 =
      CircleBuilder(w * (22.36 / mw), h * (16.83 / mh), w * (8.17 / mw / 2))
          .setFillMode(Fill::SOLID)
          .setFillColor(Colors::BLACK)
          .build();
  auto leftEye2 =
      CircleBuilder(w * (26.69 / mw), h * (20.02 / mh), w * (2.4 / mw / 2))
          .setFillMode(Fill::SOLID)
          .setFillColor(Colors::WHITE)
          .build();

  // create right eye
  auto rightEye =
      CircleBuilder(w * (53.13 / mw), h * (9.14 / mh), w * (22.12 / mw / 2))
          .setFillMode(Fill::SOLID_OUTLINED)
          .setFillColor(Colors::WHITE)
          .build();
  auto rightEye1 =
      CircleBuilder(w * (54.1 / mw), h * (16.83 / mh), w * (8.17 / mw / 2))
          .setFillMode(Fill::SOLID)
          .setFillColor(Colors::BLACK)
          .build();
  auto rightEye2 =
      CircleBuilder(w * (58.08 / mw), h * (20.02 / mh), w * (2.4 / mw / 2))
          .setFillMode(Fill::SOLID)
          .setFillColor(Colors::WHITE)
          .build();

  auto nose = EllipseBuilder(w * (43.8 / mw), h * (27.8 / mh), w * (10.58 / mw),
                             h * (5.77 / mh))
                  .setFillMode(Fill::SOLID)
                  .setFillColor(Colors::BLACK)
                  .build();

  auto moustache =
      CircleBuilder(w * (39.66 / mw), h * (32.41 / mh), w * (7.86 / mw / 2))
          .setFillMode(Fill::SOLID_OUTLINED)
          .setFillColor(Colors::GOPHER_MOUSTACHE)
          .build();
  auto moustache1 = RectBuilder(w * (43.26 / mw), h * (32.41 / mh),
                                w * (11.74 / mw), h * (5.51 / mh))
                        .setFillMode(Fill::SOLID_OUTLINED)
                        .setFillColor(Colors::GOPHER_MOUSTACHE)
                        .build();
  auto moustache2 =
      CircleBuilder(w * (51.35 / mw), h * (32.41 / mh), w * (7.86 / mw / 2))
          .setFillMode(Fill::SOLID_OUTLINED)
          .setFillColor(Colors::GOPHER_MOUSTACHE)
          .build();
  auto moustacheOverlay = RectBuilder(w * (43.26 / mw), h * (32.8 / mh),
                                      w * (11.74 / mw), h * (5 / mh))
                              .setFillMode(Fill::SOLID)
                              .setFillColor(Colors::GOPHER_MOUSTACHE)
                              .build();
  auto moustacheOverlay1 =
      CircleBuilder(w * (40 / mw), h * (32.8 / mh), w * (7 / mw / 2))
          .setFillMode(Fill::SOLID)
          .setFillColor(Colors::GOPHER_MOUSTACHE)
          .build();

  auto leftTooth = RectBuilder(w * (44.09 / mw), h * (37.2 / mh),
                               w * (4.89 / mw), h * (6.18 / mh))
                       .setFillMode(Fill::SOLID_OUTLINED)
                       .setFillColor(Colors::WHITE)
                       .build();
  auto leftTooth1 =
      CircleBuilder(w * (44.09 / mw), h * (41.25 / mh), w * (5.049 / mw / 2))
          .setFillMode(Fill::SOLID_OUTLINED)
          .setFillColor(Colors::WHITE)
          .build();
  auto leftToothOverlay = RectBuilder(w * (44.4 / mw), h * (38 / mh),
                                      w * (4.1 / mw), h * (6.18 / mh))
                              .setFillMode(Fill::SOLID)
                              .setFillColor(Colors::WHITE)
                              .build();

  auto rightTooth = RectBuilder(w * (49 / mw), h * (37 / mh), w * (4.89 / mw),
                                h * (6.18 / mh))
                        .setFillMode(Fill::SOLID_OUTLINED)
                        .setFillColor(Colors::WHITE)
                        .build();
  auto rightTooth1 =
      CircleBuilder(w * (49 / mw), h * (41.25 / mh), w * (4.89 / mw / 2))
          .setFillMode(Fill::SOLID_OUTLINED)
          .setFillColor(Colors::WHITE)
          .build();
  auto rightToothOverlay = RectBuilder(w * (49.3 / mw), h * (38 / mh),
                                       w * (4.3 / mw), h * (6.18 / mh))
                               .setFillMode(Fill::SOLID)
                               .setFillColor(Colors::WHITE)
                               .build();

  auto rightHand = EllipseBuilder(w * (87.22 / mw), h * (66.7 / mh),
                                  w * (11.47 / mw), h * (9.29 / mh))
                       .setFillMode(Fill::SOLID_OUTLINED)
                       .setFillColor(Colors::GOPHER_MOUSTACHE)
                       .build();

  auto leftHand = EllipseBuilder(w * (1.13 / mw), h * (66.7 / mh),
                                 w * (11.47 / mw), h * (9.29 / mh))
                      .setFillMode(Fill::SOLID_OUTLINED)
                      .setFillColor(Colors::GOPHER_MOUSTACHE)
                      .build();

  auto rightFoot = EllipseBuilder(w * (71.81 / mw), h * (122.06 / mh),
                                  w * (14.21 / mw), h * (7.83 / mh))
                       .setFillMode(Fill::SOLID_OUTLINED)
                       .setFillColor(Colors::GOPHER_MOUSTACHE)
                       .build();
  auto leftFoot = EllipseBuilder(w * (13.44 / mw), h * (121.19 / mh),
                                 w * (14.21 / mw), h * (7.83 / mh))
                      .setFillMode(Fill::SOLID_OUTLINED)
                      .setFillColor(Colors::GOPHER_MOUSTACHE)
                      .build();

  // create body
  auto body1 =
      EllipseBuilder(w * (10.75 / mw), 0, w * (78.86 / mw), h * (56.26 / mh))
          .setFillMode(Fill::SOLID_OUTLINED)
          .setFillColor(Colors::GOPHER)
          .build();
  auto body2 = RectBuilder(w * (10.75 / mw), h * (28.13 / mh), w * (78.86 / mw),
                           h * (75.49 / mh))
                   .setFillMode(Fill::SOLID_OUTLINED)
                   .setFillColor(Colors::GOPHER)
                   .build();
  auto body3 = EllipseBuilder(w * (10.75 / mw), h * (75.49 / mh),
                              w * (78.86 / mw), h * (56.26 / mh))
                   .setFillMode(Fill::SOLID_OUTLINED)
                   .setFillColor(Colors::GOPHER)
                   .build();
  auto bodyOverlay = RectBuilder(w * (11.3 / mw), h * (28.13 / mh),
                                 w * (77 / mw), h * (75.49 / mh))
                         .setFillMode(Fill::SOLID)
                         .setFillColor(Colors::GOPHER)
                         .build();

  auto frame = RectBuilder(0, 0, w, h)
                   .setFillMode(Fill::NONE)
                   .setBorderColor(Colors::RED)
                   .build();

  auto container = ContainerBuilder(x, y, w, h)
                       .add(leftEar)
                       .add(leftEar1)
                       .add(rightEar)
                       .add(rightEar1)
                       .add(rightHand)
                       .add(leftHand)
                       .add(rightFoot)
                       .add(leftFoot)
                       .add(body1)
                       .add(body3)
                       .add(body2)
                       .add(bodyOverlay)
                       .add(leftEye)
                       .add(leftEye1)
                       .add(leftEye2)
                       .add(rightEye)
                       .add(rightEye1)
                       .add(rightEye2)
                       .add(leftTooth)
                       .add(leftTooth1)
                       .add(leftToothOverlay)
                       .add(rightTooth)
                       .add(rightTooth1)
                       .add(rightToothOverlay)
                       .add(moustache)
                       .add(moustache1)
                       .add(moustache2)
                       .add(moustacheOverlay)
                       .add(moustacheOverlay1)
                       .add(nose)
                       //  .add(frame)
                       .build();
  return container;
}