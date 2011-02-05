#ifdef IOS
#ifndef LTIOS_H
#define LTIOS_H

#import <UIKit/UIKit.h>

void ltIOSInit();
void ltIOSTeardown();

void ltIOSRender();

void ltIOSGarbageCollect();

void ltIOSTouchesBegan(NSSet *touches);
void ltIOSTouchesMoved(NSSet *touches);
void ltIOSTouchesEnded(NSSet *touches);
void ltIOSTouchesCancelled(NSSet *touches);

#endif
#endif
