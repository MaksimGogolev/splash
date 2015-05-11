/*
 * Copyright (C) 2014 Emmanuel Durand
 *
 * This file is part of Splash.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * blobserver is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with blobserver.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * @widgets.h
 * The splash widgets
 */

#ifndef SPLASH_WIDGETS_H
#define SPLASH_WIDGETS_H

#define SPLASH_GLV_TEXTCOLOR 1.0, 1.0, 1.0
#define SPLASH_GLV_FONTSIZE 8.0
#define SPLASH_GLV_FONTWIDTH 2.0

#include <atomic>
#include <deque>
#include <functional>
#include <map>
#include <memory>
#include <unordered_map>

#include "config.h"

#include "camera.h"
#include "coretypes.h"
#include "basetypes.h"
#include "image.h"
#include "texture.h"

namespace Splash
{
class Gui;
class Scene;
typedef std::weak_ptr<Scene> SceneWeakPtr;

/*************/
class GuiWidget
{
    public:
        GuiWidget(std::string name = "");
        virtual ~GuiWidget() {}
        virtual void render() {}
        virtual int updateWindowFlags() {return 0;}

    protected:
        std::string _name {""};
};

/*************/
class GuiTextBox : public GuiWidget
{
    public:
        GuiTextBox(std::string name = "");
        void render();
        void setTextFunc(std::function<std::string()> func) {getText = func;}

    private:
        std::function<std::string()> getText;
};

/*************/
class GuiGlobalView : public GuiWidget
{
    public:
        GuiGlobalView(std::string name = "");
        void render();
        int updateWindowFlags();
        void setScene(SceneWeakPtr scene) {_scene = scene;}
        void setCamera(CameraPtr cam);
        void setObject(ObjectPtr obj);

    protected:
        CameraPtr _camera, _guiCamera;
        SceneWeakPtr _scene;
        bool _camerasHidden {false};
        bool _beginDrag {true};
        bool _noMove {false};

        // Size of the view
        int _camWidth, _camHeight;

        // Store the previous camera values
        Values _eye, _target, _up, _fov, _principalPoint;
        Values _newTarget;

        // Previous point added
        Values _previousPointAdded;

        void processKeyEvents();
        void processMouseEvents();

        // Actions
        void doCalibration();
        void propagateCalibration(); // Propagates calibration to other Scenes if needed
        void switchHideOtherCameras();
        void nextCamera();
        void showAllCalibrationPoints();
};

/*************/
class GuiControl : public GuiWidget
{
    public:
        GuiControl(std::string name) : GuiWidget(name) {}
        void render();
        int updateWindowFlags();
        void setScene(SceneWeakPtr scene) {_scene = scene;}

    private:
        SceneWeakPtr _scene;
        std::shared_ptr<GuiWidget> _nodeView;
        int _targetIndex {-1};
        std::string _targetObjectName {};
        
        std::vector<std::string> getObjectNames();
};

/*************/
class GuiGraph : public GuiWidget
{
    public:
        GuiGraph(std::string name) : GuiWidget(name) {}
        void render();

    private:
        std::atomic_uint _target;
        unsigned int _maxHistoryLength {300};
        std::unordered_map<std::string, std::deque<unsigned long long>> _durationGraph;
};

/*************/
class GuiTemplate : public GuiWidget
{
    public:
        GuiTemplate(std::string name) : GuiWidget(name) {}
        void render();
        void setScene(SceneWeakPtr scene) {_scene = scene;}

    private:
        SceneWeakPtr _scene;
        bool _templatesLoaded {false};
        std::vector<std::string> _names;
        std::map<std::string, Texture_ImagePtr> _textures;
        std::map<std::string, std::string> _descriptions;

        void loadTemplates();
};

/*************/
class GuiNodeView : public GuiWidget
{
    public:
        GuiNodeView(std::string name) : GuiWidget(name) {}
        void render();
        void setScene(SceneWeakPtr scene) {_scene = scene;}
        int updateWindowFlags();

    private:
        SceneWeakPtr _scene;
        bool _isHovered {false};

        // Node render settings
        std::vector<int> _nodeSize {160, 60};
        std::vector<int> _viewSize {640, 240};
        std::vector<int> _viewShift {0, 0};
        std::map<std::string, std::vector<float>> _nodePositions;
        
        std::map<std::string, std::vector<std::string>> getObjectLinks();
        std::map<std::string, std::string> getObjectTypes();
        void renderNode(std::string name);
};

} // end of namespace

#endif // SPLASH_WIDGETS_H
