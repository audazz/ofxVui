#pragma once

#ifndef __ofxVui_Element__
#define __ofxVui_Element__

#include "ofMain.h"
#include "ofxEasing.h"

namespace VUI {
	

	class Element : public ofNode
	{
    public:
        
        friend class VUI::EM;
        
	protected:

        struct Image {
            ofRectangle bounds;
            ofImage *image;
            ofVec2f scale;
            
            bool active = false;
            
            ImageSize size = VUI_IMAGE_NORMAL;
            
            void Set( ofImage *imagePtr, int x = 0, int y = 0, int width = -1, int height = -1){
                image = imagePtr;
                bounds.set(x, y, width, height );
                active = true;
            }
            
            void Set( ofImage *imagePtr, string value ){
                if ( value == "FILL" ) {
                    image = imagePtr;
                    size = VUI_IMAGE_FILL;
                    scale.set(1,1);
                    bounds.set(0,0, image->getWidth(), image->getHeight() );
                    active = true;
                }
            }
            
            void Clear(){
                active = false;
            }
        };
        
        map< int, Image> bgImage;

        
		string name;

		map< int, map<string, string> > style;
		map< int, map<string, float> > styleFloat;
        map< int, map<string, int> > styleInt;
        
        vector<string> borderProps = {"border-top", "border-bottom", "border-left", "border-right" };

		map< int, vector<string> > imageIDs;
		map< string, ofImage* > images;
		map< int, bool> hasState;

		int anchorPoint = VUI_ALIGN_LEFT_TOP;

		int renderState = VUI_STATE_UP;
		int virtualState = VUI_STATE_UP;
        int prevVirtualState = VUI_STATE_UP;
		bool hasStyle = false;
        
        

        bool DEBUG_MODE = false;
		
        
		ofVec3f anchorOffset;
		StyleSheet *styleSheet = nullptr;

        bool userUpdating = false;
		ofColor color;
        
        string unparsedStyle;
        
        
	public:
		virtual ~Element();
		Element( int x = 0, int y = 0, StyleSheet *ss = nullptr, string primarySelector = "", string secondarySelector = "" );
        virtual void Setup( int x = 0, int y = 0, StyleSheet *ss = nullptr, string primarySelector = "", string secondarySelector = "" );
        void SetupElement( int x = 0, int y = 0, StyleSheet *ss = nullptr, string primarySelector = "", string secondarySelector = "" );

		ofEvent<vuiEventArgs> onMouseOver;
        ofEvent<vuiEventArgs> onMouseOut;
		ofEvent<vuiEventArgs> onMousePressed;
        ofEvent<vuiEventArgs> onMouseMoved;
        ofEvent<vuiEventArgs> onMouseDragged;
		ofEvent<vuiEventArgs> onMouseReleased;
        ofEvent<vuiEventArgs> onMouseClick;
        ofEvent<vuiEventArgs> onMouseDoubleClick;

		ofEvent<vuiEventArgs> onTouchDown;
		ofEvent<vuiEventArgs> onTouchUp;
		ofEvent<vuiEventArgs> onTouchTap;
        ofEvent<vuiEventArgs> onTouchDoubleTap;
        
        ofEvent<vuiEventArgs> onStateChange;
        ofEvent<vuiEventArgs> onValueChange;
        ofEvent<vuiEventArgs> onTextChange;
        
        ofEvent<vuiEventArgs> onFocus;
        ofEvent<vuiEventArgs> onUnfocus;

		ofFbo *fbo = nullptr;
		ofTexture *maskTex = nullptr;

		int vuiUID = abs((int)ofRandom(7, 7777777777));
        
        int GetVirtualState(){
            return virtualState;
        }

        void DEBUG(){
            DEBUG_MODE = true;
        }
        
        void SetBorderRenderingOrder( string a, string b, string c, string d ){
            borderProps[0] = a;
            borderProps[1] = b;
            borderProps[2] = c;
            borderProps[3] = d;
        };
        
		void SetMask( ofTexture *maskTexture ) {
			maskTex = maskTexture;

			if (fbo != nullptr) return;

			fbo = new ofFbo();
			//fbo->allocate(styleFloat[state]["width"], styleFloat[state]["height"], GL_RGBA);
            fbo->allocate(width*scale, height*scale, GL_RGBA);
			fbo->begin();
			ofClear(255, 255, 255, 0);
			fbo->end();
		}
        
        float width = 60;
        float height = 60;
        float rotation = 0.0;
        
        float GetWidth( bool scaled = true ){
            if ( scaled ) return width*scale;
            else return width;
            //return styleFloat[state]["width"];
        }
        
        float GetHeight( bool scaled = true){
            if ( scaled ) return height*scale;
            else return height;
            //return styleFloat[state]["height"];
        }
        
        int GetState(){
            return renderState;
        }
        
        float GetRotation(){
            return rotation;
        }
        
        void SetRotation( float r ){
            rotation = r;
        }
        
        void SetState( VUI::State toState ){
            
            VUI::State s = toState;
            
            if ( s == VUI_STATE_DOWN && !hasState[VUI_STATE_OVER] ) s = VUI_STATE_OVER;
            if ( s == VUI_STATE_OVER && !hasState[VUI_STATE_OVER] ) s = VUI_STATE_UP;
            
            bool updated = false;
            
            if ( s != renderState ) renderState = s;
            
            UpdateVirtualState( toState );
        }
        
        void SetSelected(){
            SetState( VUI_STATE_DOWN );
        }
        
        void UpdateVirtualState( VUI::State toState, bool force = false ){
            if ( virtualState != toState ){
                
                prevVirtualState = int(virtualState);
                virtualState = toState;
                
                vuiEventArgs args;
                args.element = this;
                args.eventType = VUI_EVENT_STATE_CHANGE;
                args.renderState = int(toState);
                args.virtualState = int(virtualState);
                
                ofNotifyEvent( onStateChange, args, this );
                
                if ( toState == VUI_STATE_DOWN ) {
                    TriggerEvent( VUI_EVENT_VALUE_CHANGE );
                } else if ( prevVirtualState == VUI_STATE_DOWN && (toState == VUI_STATE_OVER || toState == VUI_STATE_UP) ){
                    TriggerEvent( VUI_EVENT_VALUE_CHANGE );
                }
                
                
            }
            
            if ( force ) {
                prevVirtualState = int(virtualState);
                virtualState = toState;
            }
        }
        
		int timeLastToggle = -1;
        void SetToggle( bool doToggle = true){
            isToggle = doToggle;
        }
        
        void MakeToggle(){
            SetToggle();
        }

		//

		Element* SetName(string _name) { name = _name; return this; }
        string GetName(){ return name; }

		Element* SetStyle(string style, int renderState = VUI_STATE_UP, bool initState = true );

        void Render(float parentOffsetX = 0, float parentOffsetY = 0, float parentOpacity = 1.0, ofVec2f _anchorOffset = ofVec2f::zero() );
		void Update(int mouseX = -1, int mouseY = -1, bool internalUpdate = false);
		void ParseStyle(string property = "", int renderState = VUI_STATE_UP);
        void ParseStyleSheet(StyleSheet *ss = nullptr, string primarySelector = "", string secondarySelector = "");
		float GetProperty(string property);
		void SetProperty(string property, string val);
		void SetProperty(string property, float val);
        void SetProperty(string property, int val);
        
        
        
        vector<Element*> children;
        Element* parent = nullptr;
        
        bool HasParent(){
            if ( parent == nullptr || parent == NULL ) return false;
            return true;
        }
        
        void AddChild( Element* el){
            if ( !el->HasParent() ) {
                el->SetParent( this );
                children.push_back(el);
            }
        }
        
        vector<Element*> GetChildren(){
            return children;
        }
        
        void SetParent( Element* el ){
            this->parent = el;
        }

		Element* SetSize(float w, float h);
        void SetHeight( float h );
        void SetWidth( float w );
        ofVec2f GetPosition();
		Element* SetPosition(float x, float y);
        void SetPositionX( float x );
        void SetPositionY( float y );
		void UseStyleClass(string name);
		void UseStyleID(string name);
        void UseStyle(string style);
        
        float scale = 1.0;
        
        bool isInteractive = true;
        
        void SetInteractive( bool interactive ){
            isInteractive = interactive;
        }
        
        float GetScale(){
            return scale;
        }
        
        void SetScale( float scale ){
            this->scale = scale;
        }
        
        void Set( string properties ){
            
            string s(properties);
            ofStringReplace(s, "{", " ");
            ofStringReplace(s, "}", " ");
            ofStringReplace(s, " ", "");
            
            vector<string> split = ofSplitString(s, "," );
            for ( vector<string>::iterator it = split.begin(); it != split.end(); it++){
                vector<string> propVal = ofSplitString( (*it), ":" );
                if ( propVal.size() == 2 ){
                    if ( propVal[0] == "x" ){
                        SetPositionX( ofToFloat( propVal[1] ) );
                    } else if ( propVal[0] == "y" ){
                        SetPositionY( ofToFloat( propVal[1] ) );
                    } else if ( propVal[0] == "opacity" ){
                        SetOpacity( ofToFloat( propVal[1] ) );
                    } else if ( propVal[0] == "rotation" ){
                        SetRotation( ofToFloat( propVal[1] ) );
                    } else if ( propVal[0] == "width" ){
                        SetWidth( ofToFloat( propVal[1] ) );
                    } else if ( propVal[0] == "height" ){
                        SetHeight( ofToFloat( propVal[1] ) );
                    } else if ( propVal[0] == "interactive" ){
                        if ( propVal[1] == "true" ) SetInteractive(true);
                        else if ( propVal[1] == "false" ) SetInteractive(false);
                    } else if ( propVal[0] == "scale" ){
                        SetScale( ofToFloat( propVal[1] ) );
                    }
                }
            }
        }
        
		void SetStyleSheet(StyleSheet* ss) {
			styleSheet = ss;
		}
		void SetAnchorPoint(Align alignment) {
			anchorPoint = alignment;
		}
        
        void TriggerEvent(vuiEvent eventType);
        
        vuiEventArgs GetEventArgs(vuiEvent eventType);
        
        map<string,string> propValue;
        
        void StorePropValue( string prop, string value){
            propValue[prop] = value;
        }
        
        virtual void RenderAfter(float parentOffsetX = 0, float parentOffsetY = 0){};
        
        template <typename ArgumentsType, class ListenerClass>
        Tween* Animate( float timeSeconds, string params, ListenerClass* listener, void (ListenerClass::*listenerMethod)(ArgumentsType&) ){
            //ofLog() << timeSeconds << " - " << params;
            return VUI::Animate( this, timeSeconds, params, listener, listenerMethod );
            
        }
        
        Tween* Animate( float timeSeconds, string params ){
            //ofLog() << timeSeconds << " - " << params;
            return VUI::Animate( this, timeSeconds, params );
        }
        
        void SetOpacity( float o ){
            opacity = o;
        }
        
        float GetOpacity(){
            return opacity;
        }

    protected:
        ofTrueTypeFont* font = nullptr;
        vector<string> SplitStyles(string s);
        virtual void UpdateState(int renderState, bool isInside = false, bool isMouseDown = false);
        void UpdatePosition();
        
        ofVec3f localMinPosition;
        ofVec3f localMaxPosition;
        ofVec3f posOffset;
        
        ofVec3f globalMinPosition;
        ofVec3f globalMaxPosition;
        
        ofVec3f drawPosition;
        
        float opacity = 1.0;
        float parentSumOpacity = 1.0;
        
        ofVec2f mouseDownPos;
        bool isMouseDown = false;
        bool isMouseInside = false;
        int lastClickTimeMS = 0;
		int touchDownTimeMS = 0;

		ofVec2f touchPoint;
    private:
		void SetDefaultStyles(int x, int y);
		void UpdateAnchorOffset();
        ofVec3f parentSumOffset;
        
        bool isToggle = false;
        int lastTimeMouseDown;
        
        
	};
}

#endif
