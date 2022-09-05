// Purpose: Custom and built-in UI elements.
// Created on: 8/11/2022 @ 2:48 PM

#pragma once

#include <SharedEngineCode/Internal/CppHeader.h>
#include <SharedEngineCode/Storage/DynamicArray.h>

#include "BaconEngine/Math/Vector.h"
#include "BaconEngine/Math/Color.h"
#include "BaconEngine/SDLSupport.h"
#include "BaconEngine/Event.h"

SEC_CPP_GUARD_START()
    struct BE_UIElement;
    typedef struct BE_UI_Window BE_UI_Window;

    typedef struct {
        void (*OnRender)(struct BE_UIElement* thisElement, struct BE_UI_Window* uiWindow, double deltaTime);
        int (*OnEvent)(struct BE_UIElement* thisElement, struct BE_UI_Window* uiWindow, BE_Event event);
    } BE_UIElement_Functions;

    // NOTE: This is utterly fucking stupid.
    typedef struct BE_UIElement_PublicVariables {
        void* any1; void* any2; void* any3; void* any4; void* any5;
        void* any6; void* any7; void* any8; void* any9; void* any10;

        void* (*func1)(struct BE_UIElement_PublicVariables variables); void* (*func2)(struct BE_UIElement_PublicVariables variables); void* (*func3)(struct BE_UIElement_PublicVariables variables); void* (*func4)(struct BE_UIElement_PublicVariables variables); void* (*func5)(struct BE_UIElement_PublicVariables variables);
        void* (*func6)(struct BE_UIElement_PublicVariables variables); void* (*func7)(struct BE_UIElement_PublicVariables variables); void* (*func8)(struct BE_UIElement_PublicVariables variables); void* (*func9)(struct BE_UIElement_PublicVariables variables); void* (*func10)(struct BE_UIElement_PublicVariables variables);
        
        char* str1; char* str2; char* str3; char* str4; char* str5;
        char* str6; char* str7; char* str8; char* str9; char* str10;
        
        int int1, int2, int3, int4, int5,
            int6, int7, int8, int9, int10;
        
        double double1, double2, double3, double4, double5,
               double6, double7, double8, double9, double10;
        
        float float1, float2, float3, float4, float5,
              float6, float7, float8, float9, float10;
        
        BE_Vector_2S vec2s1, vec2s2, vec2s3, vec2s4, vec2s5,
                     vec2s6, vec2s7, vec2s8, vec2s9, vec2s10;
        
        BE_Vector_2US vec2us1, vec2us2, vec2us3, vec2us4, vec2us5,
                      vec2us6, vec2us7, vec2us8, vec2us9, vec2us10;
        
        BE_Vector_2I vec21, vec22, vec23, vec24, vec25,
                     vec26, vec27, vec28, vec29, vec210;
        
        BE_Vector_2U vec2u1, vec2u2, vec2u3, vec2u4, vec2u5,
                     vec2u6, vec2u7, vec2u8, vec2u9, vec2u10;
        
        BE_Vector_2L vec2l1, vec2l2, vec2l3, vec2l4, vec2l5,
                     vec2l6, vec2l7, vec2l8, vec2l9, vec2l10;
        
        BE_Vector_2UL vec2ul1, vec2ul2, vec2ul3, vec2ul4, vec2ul5,
                      vec2ul6, vec2ul7, vec2ul8, vec2ul9, vec2ul10;
        
        BE_Vector_2LL vec2ll1, vec2ll2, vec2ll3, vec2ll4, vec2ll5,
                      vec2ll6, vec2ll7, vec2ll8, vec2ll9, vec2ll10;
        
        BE_Vector_2ULL vec2ull1, vec2ull2, vec2ull3, vec2ull4, vec2ull5,
                       vec2ull6, vec2ull7, vec2ull8, vec2ull9, vec2ull10;
        
        BE_Vector_2F vec2f1, vec2f2, vec2f3, vec2f4, vec2f5,
                     vec2f6, vec2f7, vec2f8, vec2f9, vec2f10;
        
        BE_Vector_2D vec2d1, vec2d2, vec2d3, vec2d4, vec2d5,
                     vec2d6, vec2d7, vec2d8, vec2d9, vec2d10;
        
        BE_Vector_2LD vec2ld1, vec2ld2, vec2ld3, vec2ld4, vec2ld5,
                      vec2ld6, vec2ld7, vec2ld8, vec2ld9, vec2ld10;
        
        BE_Vector_3S vec3s1, vec3s2, vec3s3, vec3s4, vec3s5,
                     vec3s6, vec3s7, vec3s8, vec3s9, vec3s10;
        
        BE_Vector_3US vec3us1, vec3us2, vec3us3, vec3us4, vec3us5,
                      vec3us6, vec3us7, vec3us8, vec3us9, vec3us10;
        
        BE_Vector_3I vec31, vec32, vec33, vec34, vec35,
                     vec36, vec37, vec38, vec39, vec310;
        
        BE_Vector_3U vec3u1, vec3u2, vec3u3, vec3u4, vec3u5,
                     vec3u6, vec3u7, vec3u8, vec3u9, vec3u10;
        
        BE_Vector_3L vec3l1, vec3l2, vec3l3, vec3l4, vec3l5,
                     vec3l6, vec3l7, vec3l8, vec3l9, vec3l10;
        
        BE_Vector_3UL vec3ul1, vec3ul2, vec3ul3, vec3ul4, vec3ul5,
                      vec3ul6, vec3ul7, vec3ul8, vec3ul9, vec3ul10;
        
        BE_Vector_3LL vec3ll1, vec3ll2, vec3ll3, vec3ll4, vec3ll5,
                      vec3ll6, vec3ll7, vec3ll8, vec3ll9, vec3ll10;
        
        BE_Vector_3ULL vec3ull1, vec3ull2, vec3ull3, vec3ull4, vec3ull5,
                       vec3ull6, vec3ull7, vec3ull8, vec3ull9, vec3ull10;
        
        BE_Vector_3F vec3f1, vec3f2, vec3f3, vec3f4, vec3f5,
                     vec3f6, vec3f7, vec3f8, vec3f9, vec3f10;
        
        BE_Vector_3D vec3d1, vec3d2, vec3d3, vec3d4, vec3d5,
                     vec3d6, vec3d7, vec3d8, vec3d9, vec3d10;
        
        BE_Vector_3LD vec3ld1, vec3ld2, vec3ld3, vec3ld4, vec3ld5,
                      vec3ld6, vec3ld7, vec3ld8, vec3ld9, vec3ld10;
        
        BE_Vector_4S vec4s1, vec4s2, vec4s3, vec4s4, vec4s5,
                     vec4s6, vec4s7, vec4s8, vec4s9, vec4s10;
        
        BE_Vector_4US vec4us1, vec4us2, vec4us3, vec4us4, vec4us5,
                      vec4us6, vec4us7, vec4us8, vec4us9, vec4us10;
        
        BE_Vector_4I vec41, vec42, vec43, vec44, vec45,
                     vec46, vec47, vec48, vec49, vec410;
        
        BE_Vector_4U vec4u1, vec4u2, vec4u3, vec4u4, vec4u5,
                     vec4u6, vec4u7, vec4u8, vec4u9, vec4u10;
        
        BE_Vector_4L vec4l1, vec4l2, vec4l3, vec4l4, vec4l5,
                     vec4l6, vec4l7, vec4l8, vec4l9, vec4l10;
        
        BE_Vector_4UL vec4ul1, vec4ul2, vec4ul3, vec4ul4, vec4ul5,
                      vec4ul6, vec4ul7, vec4ul8, vec4ul9, vec4ul10;
        
        BE_Vector_4LL vec4ll1, vec4ll2, vec4ll3, vec4ll4, vec4ll5,
                      vec4ll6, vec4ll7, vec4ll8, vec4ll9, vec4ll10;
        
        BE_Vector_4ULL vec4ull1, vec4ull2, vec4ull3, vec4ull4, vec4ull5,
                       vec4ull6, vec4ull7, vec4ull8, vec4ull9, vec4ull10;
        
        BE_Vector_4F vec4f1, vec4f2, vec4f3, vec4f4, vec4f5,
                     vec4f6, vec4f7, vec4f8, vec4f9, vec4f10;
        
        BE_Vector_4D vec4d1, vec4d2, vec4d3, vec4d4, vec4d5,
                     vec4d6, vec4d7, vec4d8, vec4d9, vec4d10;
        
        BE_Vector_4LD vec4ld1, vec4ld2, vec4ld3, vec4ld4, vec4ld5,
                      vec4ld6, vec4ld7, vec4ld8, vec4ld9, vec4ld10;

        BE_Color_3S col3s1, col3s2, col3s3, col3s4, col3s5,
                    col3s6, col3s7, col3s8, col3s9, col3s10;

        BE_Color_3US col3us1, col3us2, col3us3, col3us4, col3us5,
                     col3us6, col3us7, col3us8, col3us9, col3us10;

        BE_Color_3I col31, col32, col33, col34, col35,
                    col36, col37, col38, col39, col310;

        BE_Color_3U col3u1, col3u2, col3u3, col3u4, col3u5,
                    col3u6, col3u7, col3u8, col3u9, col3u10;

        BE_Color_3L col3l1, col3l2, col3l3, col3l4, col3l5,
                    col3l6, col3l7, col3l8, col3l9, col3l10;

        BE_Color_3UL col3ul1, col3ul2, col3ul3, col3ul4, col3ul5,
                     col3ul6, col3ul7, col3ul8, col3ul9, col3ul10;

        BE_Color_3LL col3ll1, col3ll2, col3ll3, col3ll4, col3ll5,
                     col3ll6, col3ll7, col3ll8, col3ll9, col3ll10;

        BE_Color_3ULL col3ull1, col3ull2, col3ull3, col3ull4, col3ull5,
                      col3ull6, col3ull7, col3ull8, col3ull9, col3ull10;

        BE_Color_3F col3f1, col3f2, col3f3, col3f4, col3f5,
                    col3f6, col3f7, col3f8, col3f9, col3f10;

        BE_Color_3D col3d1, col3d2, col3d3, col3d4, col3d5,
                    col3d6, col3d7, col3d8, col3d9, col3d10;

        BE_Color_3LD col3ld1, col3ld2, col3ld3, col3ld4, col3ld5,
                     col3ld6, col3ld7, col3ld8, col3ld9, col3ld10;

        BE_Color_4S col4s1, col4s2, col4s3, col4s4, col4s5,
                    col4s6, col4s7, col4s8, col4s9, col4s10;

        BE_Color_4US col4us1, col4us2, col4us3, col4us4, col4us5,
                     col4us6, col4us7, col4us8, col4us9, col4us10;

        BE_Color_4I col41, col42, col43, col44, col45,
                    col46, col47, col48, col49, col410;

        BE_Color_4U col4u1, col4u2, col4u3, col4u4, col4u5,
                    col4u6, col4u7, col4u8, col4u9, col4u10;

        BE_Color_4L col4l1, col4l2, col4l3, col4l4, col4l5,
                    col4l6, col4l7, col4l8, col4l9, col4l10;

        BE_Color_4UL col4ul1, col4ul2, col4ul3, col4ul4, col4ul5,
                     col4ul6, col4ul7, col4ul8, col4ul9, col4ul10;

        BE_Color_4LL col4ll1, col4ll2, col4ll3, col4ll4, col4ll5,
                     col4ll6, col4ll7, col4ll8, col4ll9, col4ll10;

        BE_Color_4ULL col4ull1, col4ull2, col4ull3, col4ull4, col4ull5,
                      col4ull6, col4ull7, col4ull8, col4ull9, col4ull10;

        BE_Color_4F col4f1, col4f2, col4f3, col4f4, col4f5,
                    col4f6, col4f7, col4f8, col4f9, col4f10;

        BE_Color_4D col4d1, col4d2, col4d3, col4d4, col4d5,
                    col4d6, col4d7, col4d8, col4d9, col4d10;

        BE_Color_4LD col4ld1, col4ld2, col4ld3, col4ld4, col4ld5,
                     col4ld6, col4ld7, col4ld8, col4ld9, col4ld10;
    } BE_UIElement_PublicVariables;

    typedef struct BE_UIElement {
        BE_Vector_2I position;
        BE_Vector_2U size;
        BE_Color_4U color;
        int enabled;
        int canContainElements;
        SEC_DynamicArray internalElements;
        BE_UIElement_Functions functions;
        BE_UIElement_PublicVariables variables;
    } BE_UIElement;

    BE_UIElement* BE_UIElement_Create(BE_Vector_2I position, BE_Vector_2U size, BE_Color_4U color, int enabled, BE_UIElement_Functions functions, BE_UIElement_PublicVariables variables);
    BE_UIElement* BE_UIElement_CreateLabel(const char* contents, BE_Vector_2I position);
    BE_UIElement* BE_UIElement_CreateButton(const char* contents, BE_Vector_2I position, BE_Vector_2U size, void (*UIButtonOnClick)(void));
    BE_UIElement* BE_UIElement_CreateBox(BE_Vector_2I position, BE_Vector_2U size, BE_Color_4U color);
    BE_UIElement* BE_UIElement_CreateInputBox(const char* input, const char* placeholder, BE_Vector_2I position, BE_Vector_2I size);
    int BE_UIElement_IsUserTargetingElement(BE_UIElement* element, BE_UI_Window uiWindow);
    BE_Vector_2I BE_UIElement_GetUIElementPosition(BE_UIElement* element, BE_UI_Window uiWindow);
SEC_CPP_GUARD_END()
