; Copyright (c) 2007-2009, Niels Martin Hansen
;
; Redistribution and use in source and binary forms, with or without
; modification, are permitted provided that the following conditions are met:
;
;   * Redistributions of source code must retain the above copyright notice,
;     this list of conditions and the following disclaimer.
;   * Redistributions in binary form must reproduce the above copyright notice,
;     this list of conditions and the following disclaimer in the documentation
;     and/or other materials provided with the distribution.
;   * Neither the name of the Aegisub Group nor the names of its contributors
;     may be used to endorse or promote products derived from this software
;     without specific prior written permission.
;
; THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
; AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
; IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
; ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
; LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
; CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
; SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
; INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
; CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
; ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
; POSSIBILITY OF SUCH DAMAGE.
;
; -----------------------------------------------------------------------------
;
; AEGISUB
;
; Website: http://www.aegisub.org/
; Contact: mailto:nielsm@indvikleren.dk
;


[Files]
; avisynth
DestDir: {app}; Source: src\devil.dll; Flags: ignoreversion solidbreak; Components: codec
DestDir: {app}; Source: src\avisynth.dll; Flags: ignoreversion; Components: codec
DestDir: {app}; Source: src\DirectShowSource.dll; Flags: ignoreversion; Components: codec
; ffmpegsource
DestDir: {app}; Source: src\ffms2.dll; Flags: ignoreversion; Components: codec
DestDir: {app}; Source: src\ffms2.pdb; Flags: ignoreversion; Components: codec and main/pdb
; vsfilter
DestDir: {app}; Source: src\vsfilter-aegisub32.dll; Flags: ignoreversion; Components: codec/vsfilter

[Components]
Name: codec; Description: Media formats support; Flags: fixed; Types: custom compact full
Name: codec/vsfilter; Description: VSFilter-Aegisub 2.40; Types: compact full custom; Flags: fixed

