#pragma once

namespace ImCAM
{
    class PiccoloEditor;
    class RenderCamera;
    class RenderEntity;
    enum class EditorAxisMode : int
    {
        TranslateMode = 0,
        RotateMode = 1,
        ScaleMode = 2,
        Default = 3
    };
    class EditorSceneManager
    {
    public:
        void initialize();
        void tick(float delta_time);
    };
} // namespace ImCAM
