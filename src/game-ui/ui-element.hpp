#pragma once

#include <SFML/Graphics.hpp>
#include <yoga/Yoga.h>

#include <vector>
#include <map>
#include <functional>

#include "../space_transform.hpp"

namespace space
{
    class Engine;
    class RenderCamera;

    enum class ElementType
    {
        Unknown, Root, Text, Image
    };

    class UIElement
    {
        public:
            // Types
            typedef std::function<bool (const sf::Event &)> EventHandler;

            // Fields

            // Constructor
            UIElement() : _parent(nullptr), _yogaNode(YGNodeNew()) { }
            virtual ~UIElement() { }

            // Methods
            virtual void update(Engine &engine, sf::Time dt);
            virtual void draw(Engine &engine, RenderCamera &target);
            virtual bool doesMouseHover(Engine &engine, sf::Vector2f mousePosition) const { return false; }

            const std::vector<UIElement *> &children() const { return _children; }

            void addChild(UIElement *element);
            void removeChild(UIElement *element);

            UIElement *parent() { return _parent; }
            void parent(UIElement *parent);

            YGNodeRef yogaNode() { return _yogaNode; }

            virtual ElementType elementType() const = 0;

            // Flex Direction
            void flexDirection(YGFlexDirection direction) { YGNodeStyleSetFlexDirection(_yogaNode, direction); }
            YGFlexDirection flexDirection() const { return YGNodeStyleGetFlexDirection(_yogaNode); }

            // Justify Content
            void justifyContent(YGJustify justify) { YGNodeStyleSetJustifyContent(_yogaNode, justify); }
            YGJustify justifyContent() const { return YGNodeStyleGetJustifyContent(_yogaNode); }

            // Align Content
            void alignContent(YGAlign align) { YGNodeStyleSetAlignContent(_yogaNode, align); }
            YGAlign alignContent() const { return YGNodeStyleGetAlignContent(_yogaNode); }

            // Align Items
            void alignItems(YGAlign align) { YGNodeStyleSetAlignItems(_yogaNode, align); }
            YGAlign alignItems() const { return YGNodeStyleGetAlignItems(_yogaNode); }

            // Position Type
            void positionType(YGPositionType type) { YGNodeStyleSetPositionType(_yogaNode, type); }
            YGPositionType positionType() const { return YGNodeStyleGetPositionType(_yogaNode); }

            // Flex Wrap
            void flexWrap(YGWrap wrap) { YGNodeStyleSetFlexWrap(_yogaNode, wrap); }
            YGWrap flexWrap() const { return YGNodeStyleGetFlexWrap(_yogaNode); }

            // Overflow
            void overflow(YGOverflow overflow) { YGNodeStyleSetOverflow(_yogaNode, overflow); }
            YGOverflow overflow() const { return YGNodeStyleGetOverflow(_yogaNode); }

            // Flex
            void flex(float flex) { YGNodeStyleSetFlex(_yogaNode, flex); }
            float flex() const { return YGNodeStyleGetFlex(_yogaNode); }

            // Flex Grow
            void flexGrow(float grow) { YGNodeStyleSetFlexGrow(_yogaNode, grow); }
            float flexGrow() const { return YGNodeStyleGetFlexGrow(_yogaNode); }

            // Flex Shrink
            void flexShrink(float shrink) { YGNodeStyleSetFlexShrink(_yogaNode, shrink); }
            float flexShrink() const { return YGNodeStyleGetFlexShrink(_yogaNode); }

            // Flex Basis
            void flexBasis(float basis) { YGNodeStyleSetFlexBasis(_yogaNode, basis); }
            void flexBasisPercent(float percent) { YGNodeStyleSetFlexBasisPercent(_yogaNode, percent); }
            void flexBasisAuto() { YGNodeStyleSetFlexBasisAuto(_yogaNode); }
            YGValue flexBasis() const { return YGNodeStyleGetFlexBasis(_yogaNode); }

            // Position
            void position(YGEdge edge, float position) { YGNodeStyleSetPosition(_yogaNode, edge, position); }
            void positionPercent(YGEdge edge, float position) { YGNodeStyleSetPositionPercent(_yogaNode, edge, position); }
            YGValue position(YGEdge edge) const { return YGNodeStyleGetPosition(_yogaNode, edge); }

            // Padding
            void padding(YGEdge edge, float padding) { YGNodeStyleSetPadding(_yogaNode, edge, padding); }
            void paddingPercent(YGEdge edge, float padding) { YGNodeStyleSetPaddingPercent(_yogaNode, edge, padding); }
            YGValue padding(YGEdge edge) const { return YGNodeStyleGetPadding(_yogaNode, edge); }

            // Margin
            void margin(YGEdge edge, float margin) { YGNodeStyleSetMargin(_yogaNode, edge, margin); }
            void margin(float vertical, float horizontal)
            {
                YGNodeStyleSetMargin(_yogaNode, YGEdgeVertical, vertical);
                YGNodeStyleSetMargin(_yogaNode, YGEdgeHorizontal, horizontal);
            }
            void margin(float top, float right, float bottom, float left)
            {
                YGNodeStyleSetMargin(_yogaNode, YGEdgeTop, top);
                YGNodeStyleSetMargin(_yogaNode, YGEdgeRight, right);
                YGNodeStyleSetMargin(_yogaNode, YGEdgeBottom, bottom);
                YGNodeStyleSetMargin(_yogaNode, YGEdgeLeft, left);
            }
            void marginPercent(YGEdge edge, float margin) { YGNodeStyleSetMarginPercent(_yogaNode, edge, margin); }
            void marginAuto(YGEdge edge) { YGNodeStyleSetMarginAuto(_yogaNode, edge); }
            YGValue margin(YGEdge edge) const { return YGNodeStyleGetMargin(_yogaNode, edge); }

            // Border
            void border(YGEdge edge, float border) { YGNodeStyleSetBorder(_yogaNode, edge, border); }
            float border(YGEdge edge) const { return YGNodeStyleGetBorder(_yogaNode, edge); }

            // Width
            void width(float width) { YGNodeStyleSetWidth(_yogaNode, width); }
            void widthPercent(float width) { YGNodeStyleSetWidthPercent(_yogaNode, width); }
            void widthAuto() { YGNodeStyleSetWidthAuto(_yogaNode); }
            YGValue width() const { return YGNodeStyleGetWidth(_yogaNode); }

            // Height
            void height(float height) { YGNodeStyleSetHeight(_yogaNode, height); }
            void heightPercent(float height) { YGNodeStyleSetHeightPercent(_yogaNode, height); }
            void heightAuto() { YGNodeStyleSetHeightAuto(_yogaNode); }
            YGValue height() const { return YGNodeStyleGetHeight(_yogaNode); }

            // Max Width
            void maxWidth(float maxWidth) { YGNodeStyleSetMaxWidth(_yogaNode, maxWidth); }
            void maxWidthPercent(float maxWidth) { YGNodeStyleSetMaxWidthPercent(_yogaNode, maxWidth); }
            YGValue maxWidth() const { return YGNodeStyleGetMaxWidth(_yogaNode); }

            // Max Height
            void maxHeight(float maxHeight) { YGNodeStyleSetMaxHeight(_yogaNode, maxHeight); }
            void maxHeightPercent(float maxHeight) { YGNodeStyleSetMaxHeightPercent(_yogaNode, maxHeight); }
            YGValue maxHeight() const { return YGNodeStyleGetMaxHeight(_yogaNode); }

            // Aspect Ratio
            void aspectRatio(float aspectRatio) { YGNodeStyleSetAspectRatio(_yogaNode, aspectRatio); }
            float aspectRatio() const { return YGNodeStyleGetAspectRatio(_yogaNode); }

            // Get Layout (result) Position and Size
            float layoutLeft() const { return YGNodeLayoutGetLeft(_yogaNode); }
            float layoutRight() const { return YGNodeLayoutGetRight(_yogaNode); }
            float layoutTop() const { return YGNodeLayoutGetTop(_yogaNode); }
            float layoutBottom() const { return YGNodeLayoutGetBottom(_yogaNode); }
            float layoutWidth() const { return YGNodeLayoutGetWidth(_yogaNode); }
            float layoutHeight() const { return YGNodeLayoutGetHeight(_yogaNode); }
            float layoutMargin(YGEdge edge) { return YGNodeLayoutGetMargin(_yogaNode, edge); }
            float layoutBorder(YGEdge edge) { return YGNodeLayoutGetBorder(_yogaNode, edge); }
            float layoutPadding(YGEdge edge) { return YGNodeLayoutGetPadding(_yogaNode, edge); }

        protected:
            // Fields
            YGNodeRef _yogaNode;
            std::vector<UIElement *> _children;
            UIElement *_parent;
            sf::Transform _transform;
            std::map<std::string, std::vector<EventHandler>> _eventHandlers;

            // Methods
            virtual void drawChildren(Engine &engine, RenderCamera &target);
            virtual void drawSelf(Engine &engine, RenderCamera &target) { }
    };

    class UIRootElement : public UIElement
    {
        public:
            // Fields

            // Constructor

            // Methods
            virtual ElementType elementType() const { return ElementType::Root; }
    };

} // space