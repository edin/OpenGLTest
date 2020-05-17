#pragma once

#include "TypeAliases.h"
#include "Window.h"
#include "Shader.h"
#include "Camera.h"
#include "InputController.h"
#include "GameScreen.h"
#include "Mesh.h"

namespace Nuts {
    Size GameScreen::GetScreenSize() {
        Size size = window->GetSize();
        size.width = (int)(size.width * placement.Width());
        size.height = (int)(size.height * placement.Height());
        return size;
    }

    ViewPort GameScreen::GetScreenViewPort() {
        ViewPort viewPort;
        Size size = window->GetSize();
        viewPort.x = (int)(placement.left * size.width);
        viewPort.y = (int)(placement.top * size.height);

        int right = (int)(placement.right * size.width);
        int bottom = (int)(placement.bottom * size.height);

        viewPort.with = right - viewPort.x;
        viewPort.height = bottom - viewPort.y;

        return viewPort;
    }
}