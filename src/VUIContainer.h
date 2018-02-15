//
//  VUIContainer.h
//
//  Created by Christopher Miles on 2/13/18
//
//

#pragma once

#ifndef __ofxVui_Container__
#define __ofxVui_Container__

#include "ofMain.h"

namespace VUI {
    
    class Container : public Element
    {
    public:
        ~Container(){};
        
        Container( int x, int y, StyleSheet *ss = nullptr, string primarySelector = "", string secondarySelector = "" ):Element(x,y,ss,primarySelector,secondarySelector){
            Setup();
        }
        
        Container( int x, string y, StyleSheet *ss = nullptr, string primarySelector = "", string secondarySelector = "" ):Element(x,y,ss,primarySelector,secondarySelector){
            Setup();
        }
        
        Container( string x, int y, StyleSheet *ss = nullptr, string primarySelector = "", string secondarySelector = "" ):Element(x,y,ss,primarySelector,secondarySelector){
            Setup();
        }
        
        Container( string x, string y, StyleSheet *ss = nullptr, string primarySelector = "", string secondarySelector = "" ):Element(x,y,ss,primarySelector,secondarySelector){
            Setup();
        }
        
        void SetScrollbar( StyleSheet *ss, string styleSelector );
        
        void SetPadding( int top, int right, int bottom, int left ){
            padding.Set(top,right,bottom,left);
            UpdateContainerStyle();
        }
        
        void SetPadding( int pad ){
            padding.Set(pad,pad,pad,pad);
            UpdateContainerStyle();
        }
        
        void SetPadding( int topBottom, int leftRight ){
            padding.Set(topBottom,leftRight,topBottom,leftRight);
            UpdateContainerStyle();
        }
        
        void ResizeToContent(){
            SetHeight(stackPos.y+padding.top+padding.bottom);
            UpdateContainerStyle();
            UpdateScrollbarStyle();
            vertContainer->RemoveMask();
            
#ifdef USING_ofxTouchPadScroll
            ofRemoveListener( GetEventManager()->onTouchPadScroll, this, &Container::_vuiEventHandler );
#endif
        }
        
        void AddAndStackChild(Element* child, Stack stackDirection = VUI_STACK_VERT){
            
            if ( stackPos.y != 0 ) stackPos.y += margin.y;
            
            if ( stackDirection == VUI_STACK_VERT ) {
                AddChild(child);
                child->SetPosition(0,stackPos.y);
                
                stackPos.y += child->GetOriginalHeight();
            } else {
                // TODO: Test/Fix this
                
                int x = stackPos.x + child->GetWidth();
                if ( stackPos.x != 0 ) x += margin.x;
                
                if ( x <= container->GetWidth() ){
                    
                    AddChild(child);
                    child->SetPosition(stackPos.x, stackPos.y);
                    
                    stackPos.x += x;
                } else if ( child->GetWidth() <= container->GetWidth() ) {
                    stackPos.x = 0;
                    stackPos.y += child->GetOriginalHeight();
                    
                    AddAndStackChild(child, stackDirection);
                }
                
            }
            
            if ( stackPos.y > vertContainer->GetOriginalHeight() - padding.top ) CreateMask();
            UpdateScrollbarStyle();
            
        }
        
        void SetEventManager(VUI::EM* eventManager){
            Element::SetEventManager(eventManager);
            
            if ( horzContainer != NULL ){
                horzContainer->SetEventManager(eventManager);
                vertContainer->SetEventManager(eventManager);
                container->SetEventManager(eventManager);
                scrollbar->SetEventManager(eventManager);
            }
        }
        
        void Setup();
        
        void SetStackMargin(int x, int y){
            margin.x = x;
            margin.y = y;
        }
        
        void AddChild( Element* el ){
            container->AddChild(el);
        }
        
    //private:
        Padding padding;
        Element* scrollbar = NULL;
        Element* horzContainer = NULL;
        Element* vertContainer = NULL;
        Element* container = NULL;
        StyleSheet* ss = NULL;
        ofVec2f stackPos;
        ofVec2f prevMargin;
        ofVec2f margin;
        ofTexture* mask;
        ofVec2f startScrollPos;
        ofVec2f tempScrollPos;
        ofVec2f scrollDist;
        
        int scrollbarDist = 0;
        int scrollbarPadding = 2;
        int scrollbarWidth = 8;
        int minScrollbarHeight = 60;
        
    private:
        
        void CreateMask(){
            if ( vertContainer == NULL ) return;
            
            bool create = false;
            
            /*if ( texture == NULL ) create = true;
            else {
                if ( texture->getWidth() != vertContainer->GetWidth() && texture->getHeight() != vertContainer->GetHeight() ) create = true;
            }*/
            
            if ( !vertContainer->HasMask() ) create = true;
            else {
                if ( vertContainer->GetMask()->getWidth() != vertContainer->GetWidth() && vertContainer->GetMask()->getHeight() != vertContainer->GetHeight() ) create = true;
            }
            
            container->SetHeight(stackPos.y+padding.bottom);
            
            
            scrollDist.y = (stackPos.y*VUI::dpi - vertContainer->GetHeight()) + padding.bottom*VUI::dpi;

            
            if ( create ) {
                ofFbo fbo;
                fbo.allocate( vertContainer->GetWidth(), vertContainer->GetHeight(), GL_RGBA );
                fbo.begin();
                ofClear(255, 255, 255, 0);
                ofSetColor(255,255,255);
                ofDrawRectangle(0,0,vertContainer->GetWidth(), vertContainer->GetHeight() );
                fbo.end();
                
                mask = new ofTexture(fbo.getTexture());
                vertContainer->SetMask(mask);
                
#ifdef USING_ofxTouchPadScroll
                ofAddListener( GetEventManager()->onTouchPadScroll, this, &Container::_vuiEventHandler );
#endif
            }
        }
        
        void UpdateScrollbarStyle(){
            minScrollbarHeight = vertContainer->GetOriginalHeight()*.1;
            if ( minScrollbarHeight < 50 ) minScrollbarHeight = 50;
            
            scrollbar->SetPosition("calc(100%-" + ofToString(scrollbarPadding+scrollbarWidth) + ")", scrollbarPadding);
            
            int h = (vertContainer->GetOriginalHeight() / (stackPos.y*.8)) * vertContainer->GetOriginalHeight();
            if ( h < minScrollbarHeight ) h = minScrollbarHeight;
            
            scrollbar->SetHeight(h);
            scrollbar->SetWidth(scrollbarWidth);
            
            scrollbarDist = vertContainer->GetOriginalHeight() - (scrollbarPadding*2) - h;
            
            scrollbar->SetOpacity(0);
        }
        
        void UpdateContainerStyle(){
            horzContainer->Setup(padding.left, 0 );
            horzContainer->SetWidth("calc(100%-"+ofToString(padding.right)+")");
            
            vertContainer->SetWidth("calc(100%-" + ofToString(padding.right) + ")" );
            vertContainer->SetHeight("100%");
            
            container->Setup(0, padding.top );
            container->SetWidth("100%");
        }
        
        void _vuiEventHandler(vuiEventArgs& evt){
            if ( !IsMouseInside() ) return;
            
            if ( evt.eventType == VUI_EVENT_TOUCHPAD_SCROLL_END ){
                scrollbar->Animate(.3,"{delay:.4,opacity:0}");
                return;
            }
            
            if ( evt.eventType == VUI_EVENT_TOUCHPAD_SCROLL_START ){
                scrollbar->StopTween();
                startScrollPos = container->GetPosition();
                scrollbar->SetOpacity(1.0);
            } else if ( evt.eventType == VUI_EVENT_TOUCHPAD_SCROLL_INERTIA || evt.eventType == VUI_EVENT_TOUCHPAD_SCROLL ) {
                if ( evt.eventType == VUI_EVENT_TOUCHPAD_SCROLL_INERTIA ) startScrollPos = container->GetPosition();
                tempScrollPos.set(startScrollPos);
                
                tempScrollPos.y += evt.touchPadScroll.y*VUI::divideDpi;
                
                if ( tempScrollPos.y < -scrollDist.y ) tempScrollPos.y = -scrollDist.y;
                else if ( tempScrollPos.y > padding.top*VUI::dpi ) tempScrollPos.y = padding.top*VUI::dpi;
                
                container->SetPositionY(tempScrollPos.y*VUI::divideDpi);
                
                float perc = ((tempScrollPos.y)/(-scrollDist.y+padding.top*VUI::dpi));
                if ( perc < 0.0 ) perc = 0.0;
                
                scrollbar->SetPositionY( (perc*scrollbarDist) + scrollbarPadding );
                
                
            }
            
        }
        
        
    };
    
}


#endif
