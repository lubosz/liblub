/*
 * Copyright © 2010 Lubosz Sarnecki
 * Input.cpp
 *
 *  Created on: Sep 28, 2010
 */
#include <list>
#include "Scene/Camera.h"
#include "Window/MediaLayer.h"
#include "Material/Uniform.h"
#include "Scene/SceneData.h"
#include "System/Logger.h"

Input::Input(xcb_connection_t *connection) {
  this->connection = connection;
  syms = xcb_key_symbols_alloc(connection);
  pressedKeys = QList<xcb_keysym_t>();

  // TODO(bmonkey): Hardcoded values => xml
  inputSpeed = .01;
  mouseSensitivity = .1;
}

Input::~Input() {
  // TODO(bmonkey): Auto-generated destructor stub
}

void Input::eventLoop() {
  xcb_keysym_t pressedKey;
  xcb_generic_event_t *event;
  xcb_motion_notify_event_t *motion;
  xcb_key_press_event_t *kp;
  xcb_key_release_event_t *kr;

  while ((event = xcb_poll_for_event(connection))) {
    switch (event->response_type & ~0x80) {
        case XCB_MOTION_NOTIFY:
            motion = reinterpret_cast<xcb_motion_notify_event_t *>(event);
            MediaLayer::Instance().mouseLook(motion->event_x, motion->event_y);
            break;

        case XCB_KEY_RELEASE:

            kr = reinterpret_cast<xcb_key_release_event_t *>(event);
            pressedKey = xcb_key_symbols_get_keysym(syms, kr->detail, 0);
            pressedKeys.removeAll(pressedKey);

            switch (pressedKey) {
            case XK_Shift_L:
              inputSpeed = .01;
                          break;

            }
            break;

        case XCB_KEY_PRESS:
            kp = reinterpret_cast<xcb_key_press_event_t *>(event);
            pressedKey = xcb_key_symbols_get_keysym(syms, kp->detail, 0);

            switch (pressedKey) {
                case XK_Escape:
                    MediaLayer::Instance().shutdown();
                    break;
                case XK_m:
                    MediaLayer::Instance().toggleMouseGrab();
                    break;
                case XK_f:
                    MediaLayer::Instance().toggleFullScreen();
                    break;
                case XK_p:
                    RenderEngine::Instance().toggleFBO();
                    break;
                case XK_c:
                    RenderEngine::Instance().toggleLightView();
                    break;
                case XK_Tab:
                    RenderEngine::Instance().toggleWire();
                    break;
                case XK_Shift_L:
                    inputSpeed = .1;
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

void Input::checkKey(xcb_keysym_t pressedKey) {

//		uvmoveprog =
//						SceneGraph::Instance().getNode("uvmovenode")
//						->getMaterial()->shaderProgram;
    switch (pressedKey) {
      case XK_i:
        SceneData::Instance().getProgram("uvmove")->translateUniformf(0,{0,inputSpeed/10.0f});
        break;
      case XK_j:
        SceneData::Instance().getProgram("uvmove")->translateUniformf(0,{-inputSpeed/10.0f,0});
        break;
      case XK_k:
        SceneData::Instance().getProgram("uvmove")->translateUniformf(0,{0,-inputSpeed/10.0f});
        break;
      case XK_l:
        SceneData::Instance().getProgram("uvmove")->translateUniformf(0,{inputSpeed/10.0f,0});
        break;
      case XK_u:
        SceneData::Instance().getProgram("uvmove")->translateUniformf(1,{inputSpeed/10.0f});
        break;
      case XK_o:
        SceneData::Instance().getProgram("uvmove")->translateUniformf(1,{-inputSpeed/10.0f});
        break;
        case XK_w:
            SceneData::Instance().getCurrentCamera()->forwardDirection(inputSpeed);
            break;
        case XK_a:
            SceneData::Instance().getCurrentCamera()->leftDirection(inputSpeed);
            break;
        case XK_s:
            SceneData::Instance().getCurrentCamera()->backwardDirection(inputSpeed);
            break;
        case XK_d:
            SceneData::Instance().getCurrentCamera()->rightDirection(inputSpeed);
            break;
            // Light
        case XK_Left:
            SceneData::Instance().getShadowLight()->leftWorld(inputSpeed);
            break;
        case XK_Right:
            SceneData::Instance().getShadowLight()->rightWorld(inputSpeed);
            break;
        case XK_Up:
            SceneData::Instance().getShadowLight()->upWorld(inputSpeed);
            break;
        case XK_Down:
            SceneData::Instance().getShadowLight()->downWorld(inputSpeed);
            break;
        case XK_1:
            SceneData::Instance().getShadowLight()->forwardWorld(inputSpeed);
            break;
        case XK_7:
            SceneData::Instance().getShadowLight()->backWorld(inputSpeed);
            break;
    }
}
