/*
 * Copyright © 2010 Lubosz Sarnecki
 * Input.cpp
 *
 *  Created on: Sep 28, 2010
 */
#include "XCBInput.h"
#include <X11/keysym.h>
#include <list>
#include "Scene/Camera.h"
#include "Window/Window.h"
#include "Shader/Uniform.h"
#include "Scene/Scene.h"
#include "System/Logger.h"
#include "Renderer/OpenGL.h"
#include "XCBWindow.h"

XCBInput::XCBInput(xcb_connection_t *connection, XCBWindow * mediaLayer) {
  lastEventX = 0;
  lastEventY = 0;
  this->connection = connection;
  syms = xcb_key_symbols_alloc(connection);
  pressedKeys = QList<xcb_keysym_t>();

  // TODO(bmonkey): Hardcoded values => xml
  inputSpeed = .03;
  mouseSensitivity = .1;
  this->mediaLayer = mediaLayer;

  eventTimer = new QTimer(this);
  connect(eventTimer, SIGNAL(timeout()), this, SLOT(eventLoop()));
  eventTimer->start(0);
}

XCBInput::~XCBInput() {
    delete eventTimer;
    xcb_key_symbols_free(syms);
}

void XCBInput::eventLoop() {
  xcb_keysym_t pressedKey;
  xcb_generic_event_t *event;
  xcb_motion_notify_event_t *motion;
  xcb_key_press_event_t *kp;
  xcb_key_release_event_t *kr;

  bool touchPadHack = false;

  while ((event = xcb_poll_for_event(connection))) {
    switch (event->response_type & ~0x80) {
        case XCB_MOTION_NOTIFY:
            motion = reinterpret_cast<xcb_motion_notify_event_t *>(event);
            if ((lastEventX != motion->event_x || motion->event_y != lastEventY) || !touchPadHack) {
                mediaLayer->mouseLook(motion->event_x, motion->event_y);
                lastEventX = motion->event_x;
                lastEventY = motion->event_y;
            }
            break;

        case XCB_KEY_RELEASE:

            kr = reinterpret_cast<xcb_key_release_event_t *>(event);
            pressedKey = xcb_key_symbols_get_keysym(syms, kr->detail, 0);
            pressedKeys.removeAll(pressedKey);

            switch (pressedKey) {
            case XK_Shift_L:
              inputSpeed = .03;
                          break;

            }
            break;

        case XCB_KEY_PRESS:
            kp = reinterpret_cast<xcb_key_press_event_t *>(event);
            pressedKey = xcb_key_symbols_get_keysym(syms, kp->detail, 0);
            Camera * cam;

            switch (pressedKey) {
                case XK_Escape:
                    emit shutdown();
                    break;
                case XK_m:
                  mediaLayer->toggleMouseGrab();
                    break;
                case XK_f:
                  mediaLayer->toggleFullScreen();
                    break;
                case XK_p:
                    cam = Scene::Instance().getCurrentCamera();
                    LogInfo << "Cam Pos" << cam->position.x() << cam->position.y() << cam->position.z();
                    LogInfo << "Cam Dir" << cam->direction().x() << cam->direction().y() << cam->direction().z();
                    break;
                case XK_c:
                    OpenGL::Instance().toggleLightView();
                    break;
                case XK_Tab:
                    OpenGL::Instance().toggleWire();
                    break;
                case XK_Shift_L:
                    inputSpeed = .2;
                	break;
                	
                default:
                    pressedKeys.push_back(pressedKey);
            }
            break;

        default:
            /* Unknown event type, ignore it */
            // printf ("Unknown event: %d\n", event->response_type);
            break;
    }

      free(event);
  }

  foreach(xcb_keysym_t key, pressedKeys) {
      checkKey(key);
    }
}

void XCBInput::checkKey(xcb_keysym_t pressedKey) {
    switch (pressedKey) {
        case XK_w:
            Scene::Instance().getCurrentCamera()->forwardDirection(inputSpeed);
            break;
        case XK_a:
            Scene::Instance().getCurrentCamera()->leftDirection(inputSpeed);
            break;
        case XK_s:
            Scene::Instance().getCurrentCamera()->backwardDirection(inputSpeed);
            break;
        case XK_d:
            Scene::Instance().getCurrentCamera()->rightDirection(inputSpeed);
            break;
            // Light
        case XK_Left:
            Scene::Instance().getShadowLight()->leftWorld(inputSpeed);
            break;
        case XK_Right:
            Scene::Instance().getShadowLight()->rightWorld(inputSpeed);
            break;
        case XK_Up:
            Scene::Instance().getShadowLight()->upWorld(inputSpeed);
            break;
        case XK_Down:
            Scene::Instance().getShadowLight()->downWorld(inputSpeed);
            break;
        case XK_1:
            Scene::Instance().getShadowLight()->forwardWorld(inputSpeed);
            break;
        case XK_7:
            Scene::Instance().getShadowLight()->backWorld(inputSpeed);
            break;
    }
}
