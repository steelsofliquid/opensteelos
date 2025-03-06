 
#ifndef __OSOS__GUI__WIDGET_H
#define __OSOS__GUI__WIDGET_H

#include <common\types.h>
#include <common\graphicscontext.h>

namespace osos
{
    namespace gui
    {
            class Widget
            {
            protected:
                Widget* parent;

                osos::common::int32_t x;
                osos::common::int32_t y;
                osos::common::int32_t w;
                osos::common::int32_t h;

                osos::common::uint8_t r;
                osos::common::uint8_t g;
                osos::common::uint8_t b;
                bool Focussable;

            public:
                Widget(
                    Widget* parent,
                    osos::common::int32_t x,
                    osos::common::int32_t y,
                    osos::common::int32_t w,
                    osos::common::int32_t h,
                    osos::common::uint8_t r,
                    osos::common::uint8_t g,
                    osos::common::uint8_t b
                    );
                ~Widget();
            
                virtual void GetFocus(Widget* widget);
                virtual void ModelToScreen(osos::common::int32_t &x, osos::common::int32_t& y);

                virtual void Draw(GraphicsContext* gc);
                virtual void OnMouseDown(osos::common::int32_t x, osos::common::int32_t y);
                virtual void OnMouseUp(osos::common::int32_t x, osos::common::int32_t y);
                virtual void OnMouseMove(osos::common::int32_t oldx, osos::common::int32_t oldy, osos::common::int32_t newx, osos::common::int32_t newy);

                virtual void OnKeyDown(osos::common::int32_t x, osos::common::int32_t y);
                virtual void OnKeyUp(osos::common::int32_t x, osos::common::int32_t y);
            };

            class CompositeWidget : public Widget
            {
            private: 
                Widget* children[100];
                int numChildren;
                Widget* focussedChild;

            public:
                CompositeWidget(Widget* parent,
                    osos::common::int32_t x, osos::common::int32_t y, osos::common::int32_t w, osos::common::int32_t h,
                    osos::common::uint8_t r, osos::common::uint8_t g, osos::common::uint8_t b);
                ~CompositeWidget();

                virtual void GetFocus(Widget* widget);

                virtual void Draw(GraphicsContext* gc);
                virtual void OnMouseDown(osos::common::int32_t x, osos::common::int32_t y);
                virtual void OnMouseUp(osos::common::int32_t x, osos::common::int32_t y);
                virtual void OnMouseMove(osos::common::int32_t oldx, osos::common::int32_t oldy, 
                    osos::common::int32_t newx, osos::common::int32_t newy);

                virtual void OnKeyDown(osos::common::int32_t x, osos::common::int32_t y);
                virtual void OnKeyUp(osos::common::int32_t x, osos::common::int32_t y);

            };
        }
    }


#endif