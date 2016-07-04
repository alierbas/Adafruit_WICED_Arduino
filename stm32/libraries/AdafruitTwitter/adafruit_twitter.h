/**************************************************************************/
/*!
    @file     adafruit_twitter.h
    @author   hathach

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2016, Adafruit Industries (adafruit.com)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**************************************************************************/

#ifndef _ADAFRUIT_TWITTER_H_
#define _ADAFRUIT_TWITTER_H_

#include <Arduino.h>
#include <Client.h>
#include <IPAddress.h>
#include <adafruit_feather.h>
#include <adafruit_http.h>

class AdafruitTwitter :  public AdafruitSDEP
{
protected:
  char const * _consumer_key;
  char const * _consumer_secret;
  char const * _token_access;
  char const * _token_secret;

  void reset(void);

public:
  AdafruitTwitter(void);

  bool begin(char const* consumer_key, char const* consumer_secret, char const* token_access, char const * token_secret);
  bool stop(void);

  bool tweet(char const* status);

  virtual void err_actions (bool print, bool halt)
  {
//    _http.err_actions(print, halt);
    _err_print = print;
    _err_halt  = halt;
  }
};

#endif /* _ADAFRUIT_TWITTER_H_ */
