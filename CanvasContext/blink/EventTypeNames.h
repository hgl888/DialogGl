/*
 * Copyright (C) 2013 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef EventTypeNames_h
#define EventTypeNames_h
#include "wtf/text/AtomicString.h"

namespace WebCore {
namespace EventTypeNames {

#ifndef EVENT_TYPE_NAMES_HIDE_GLOBALS
extern const WTF::AtomicString DOMActivate;
extern const WTF::AtomicString DOMCharacterDataModified;
extern const WTF::AtomicString DOMContentLoaded;
extern const WTF::AtomicString DOMFocusIn;
extern const WTF::AtomicString DOMFocusOut;
extern const WTF::AtomicString DOMNodeInserted;
extern const WTF::AtomicString DOMNodeInsertedIntoDocument;
extern const WTF::AtomicString DOMNodeRemoved;
extern const WTF::AtomicString DOMNodeRemovedFromDocument;
extern const WTF::AtomicString DOMSubtreeModified;
extern const WTF::AtomicString abort;
extern const WTF::AtomicString activate;
extern const WTF::AtomicString addsourcebuffer;
extern const WTF::AtomicString addstream;
extern const WTF::AtomicString addtrack;
extern const WTF::AtomicString animationend;
extern const WTF::AtomicString animationiteration;
extern const WTF::AtomicString animationstart;
extern const WTF::AtomicString audioend;
extern const WTF::AtomicString audioprocess;
extern const WTF::AtomicString audiostart;
extern const WTF::AtomicString autocomplete;
extern const WTF::AtomicString autocompleteerror;
extern const WTF::AtomicString beforecopy;
extern const WTF::AtomicString beforecut;
extern const WTF::AtomicString beforeload;
extern const WTF::AtomicString beforepaste;
extern const WTF::AtomicString beforeunload;
extern const WTF::AtomicString beginEvent;
extern const WTF::AtomicString blocked;
extern const WTF::AtomicString blur;
extern const WTF::AtomicString boundary;
extern const WTF::AtomicString cached;
extern const WTF::AtomicString cancel;
extern const WTF::AtomicString candidatewindowhide;
extern const WTF::AtomicString candidatewindowshow;
extern const WTF::AtomicString candidatewindowupdate;
extern const WTF::AtomicString canplay;
extern const WTF::AtomicString canplaythrough;
extern const WTF::AtomicString change;
extern const WTF::AtomicString chargingchange;
extern const WTF::AtomicString chargingtimechange;
extern const WTF::AtomicString checking;
extern const WTF::AtomicString click;
extern const WTF::AtomicString close;
extern const WTF::AtomicString complete;
extern const WTF::AtomicString compositionend;
extern const WTF::AtomicString compositionstart;
extern const WTF::AtomicString compositionupdate;
extern const WTF::AtomicString connect;
extern const WTF::AtomicString connecting;
extern const WTF::AtomicString contextmenu;
extern const WTF::AtomicString copy;
extern const WTF::AtomicString cuechange;
extern const WTF::AtomicString cut;
extern const WTF::AtomicString datachannel;
extern const WTF::AtomicString dblclick;
extern const WTF::AtomicString devicemotion;
extern const WTF::AtomicString deviceorientation;
extern const WTF::AtomicString dischargingtimechange;
extern const WTF::AtomicString disconnect;
extern const WTF::AtomicString display;
extern const WTF::AtomicString downloading;
extern const WTF::AtomicString drag;
extern const WTF::AtomicString dragend;
extern const WTF::AtomicString dragenter;
extern const WTF::AtomicString dragleave;
extern const WTF::AtomicString dragover;
extern const WTF::AtomicString dragstart;
extern const WTF::AtomicString drop;
extern const WTF::AtomicString durationchange;
extern const WTF::AtomicString emptied;
extern const WTF::AtomicString end;
extern const WTF::AtomicString endEvent;
extern const WTF::AtomicString ended;
extern const WTF::AtomicString enter;
extern const WTF::AtomicString error;
extern const WTF::AtomicString exit;
extern const WTF::AtomicString fetch;
extern const WTF::AtomicString focus;
extern const WTF::AtomicString focusin;
extern const WTF::AtomicString focusout;
extern const WTF::AtomicString gesturescrollend;
extern const WTF::AtomicString gesturescrollstart;
extern const WTF::AtomicString gesturescrollupdate;
extern const WTF::AtomicString gestureshowpress;
extern const WTF::AtomicString gesturetap;
extern const WTF::AtomicString gesturetapdown;
extern const WTF::AtomicString gesturetapunconfirmed;
extern const WTF::AtomicString hashchange;
extern const WTF::AtomicString icecandidate;
extern const WTF::AtomicString iceconnectionstatechange;
extern const WTF::AtomicString input;
extern const WTF::AtomicString install;
extern const WTF::AtomicString invalid;
extern const WTF::AtomicString keydown;
extern const WTF::AtomicString keypress;
extern const WTF::AtomicString keyup;
extern const WTF::AtomicString levelchange;
extern const WTF::AtomicString load;
extern const WTF::AtomicString loadeddata;
extern const WTF::AtomicString loadedmetadata;
extern const WTF::AtomicString loadend;
extern const WTF::AtomicString loading;
extern const WTF::AtomicString loadingdone;
extern const WTF::AtomicString loadingerror;
extern const WTF::AtomicString loadstart;
extern const WTF::AtomicString mark;
extern const WTF::AtomicString message;
extern const WTF::AtomicString midimessage;
extern const WTF::AtomicString mousedown;
extern const WTF::AtomicString mouseenter;
extern const WTF::AtomicString mouseleave;
extern const WTF::AtomicString mousemove;
extern const WTF::AtomicString mouseout;
extern const WTF::AtomicString mouseover;
extern const WTF::AtomicString mouseup;
extern const WTF::AtomicString mousewheel;
extern const WTF::AtomicString mute;
extern const WTF::AtomicString needkey;
extern const WTF::AtomicString negotiationneeded;
extern const WTF::AtomicString nomatch;
extern const WTF::AtomicString noupdate;
extern const WTF::AtomicString obsolete;
extern const WTF::AtomicString offline;
extern const WTF::AtomicString online;
extern const WTF::AtomicString open;
extern const WTF::AtomicString orientationchange;
extern const WTF::AtomicString overflowchanged;
extern const WTF::AtomicString pagehide;
extern const WTF::AtomicString pageshow;
extern const WTF::AtomicString paste;
extern const WTF::AtomicString pause;
extern const WTF::AtomicString play;
extern const WTF::AtomicString playing;
extern const WTF::AtomicString popstate;
extern const WTF::AtomicString progress;
extern const WTF::AtomicString ratechange;
extern const WTF::AtomicString ready;
extern const WTF::AtomicString readystatechange;
extern const WTF::AtomicString removesourcebuffer;
extern const WTF::AtomicString removestream;
extern const WTF::AtomicString removetrack;
extern const WTF::AtomicString repeatEvent;
extern const WTF::AtomicString reset;
extern const WTF::AtomicString resize;
extern const WTF::AtomicString result;
extern const WTF::AtomicString resume;
extern const WTF::AtomicString scroll;
extern const WTF::AtomicString search;
extern const WTF::AtomicString securitypolicyviolation;
extern const WTF::AtomicString seeked;
extern const WTF::AtomicString seeking;
extern const WTF::AtomicString select;
extern const WTF::AtomicString selectionchange;
extern const WTF::AtomicString selectstart;
extern const WTF::AtomicString show;
extern const WTF::AtomicString signalingstatechange;
extern const WTF::AtomicString soundend;
extern const WTF::AtomicString soundstart;
extern const WTF::AtomicString sourceclose;
extern const WTF::AtomicString sourceended;
extern const WTF::AtomicString sourceopen;
extern const WTF::AtomicString speechend;
extern const WTF::AtomicString speechstart;
extern const WTF::AtomicString stalled;
extern const WTF::AtomicString start;
extern const WTF::AtomicString storage;
extern const WTF::AtomicString submit;
extern const WTF::AtomicString success;
extern const WTF::AtomicString suspend;
extern const WTF::AtomicString textInput;
extern const WTF::AtomicString timeout;
extern const WTF::AtomicString timeupdate;
extern const WTF::AtomicString tonechange;
extern const WTF::AtomicString touchcancel;
extern const WTF::AtomicString touchend;
extern const WTF::AtomicString touchmove;
extern const WTF::AtomicString touchstart;
extern const WTF::AtomicString transitionend;
extern const WTF::AtomicString unload;
extern const WTF::AtomicString unmute;
extern const WTF::AtomicString update;
extern const WTF::AtomicString updateend;
extern const WTF::AtomicString updateready;
extern const WTF::AtomicString updatestart;
extern const WTF::AtomicString upgradeneeded;
extern const WTF::AtomicString versionchange;
extern const WTF::AtomicString visibilitychange;
extern const WTF::AtomicString voiceschanged;
extern const WTF::AtomicString volumechange;
extern const WTF::AtomicString waiting;
extern const WTF::AtomicString webglcontextcreationerror;
extern const WTF::AtomicString webglcontextlost;
extern const WTF::AtomicString webglcontextrestored;
extern const WTF::AtomicString webkitAnimationEnd;
extern const WTF::AtomicString webkitAnimationIteration;
extern const WTF::AtomicString webkitAnimationStart;
extern const WTF::AtomicString webkitBeforeTextInserted;
extern const WTF::AtomicString webkitEditableContentChanged;
extern const WTF::AtomicString webkitTransitionEnd;
extern const WTF::AtomicString webkitaddsourcebuffer;
extern const WTF::AtomicString webkitdeviceproximity;
extern const WTF::AtomicString webkitfullscreenchange;
extern const WTF::AtomicString webkitfullscreenerror;
extern const WTF::AtomicString webkitkeyadded;
extern const WTF::AtomicString webkitkeyerror;
extern const WTF::AtomicString webkitkeymessage;
extern const WTF::AtomicString webkitneedkey;
extern const WTF::AtomicString webkitnetworkinfochange;
extern const WTF::AtomicString webkitpointerlockchange;
extern const WTF::AtomicString webkitpointerlockerror;
extern const WTF::AtomicString webkitprerenderdomcontentloaded;
extern const WTF::AtomicString webkitprerenderload;
extern const WTF::AtomicString webkitprerenderstart;
extern const WTF::AtomicString webkitprerenderstop;
extern const WTF::AtomicString webkitremovesourcebuffer;
extern const WTF::AtomicString webkitresourcetimingbufferfull;
extern const WTF::AtomicString webkitsourceclose;
extern const WTF::AtomicString webkitsourceended;
extern const WTF::AtomicString webkitsourceopen;
extern const WTF::AtomicString webkitspeechchange;
extern const WTF::AtomicString webkitvisibilitychange;
extern const WTF::AtomicString wheel;
extern const WTF::AtomicString write;
extern const WTF::AtomicString writeend;
extern const WTF::AtomicString writestart;
extern const WTF::AtomicString zoom;

#endif // EVENT_TYPE_NAMES_HIDE_GLOBALS

 void init();

} // EventTypeNames
} // WebCore

#endif
