#ifdef LTIOS
#import <GameKit/GameKit.h>
#include "lt.h"

static bool game_center_available = false;
static bool game_center_supported = false;

@interface GameCenterLeaderboardViewController : GKLeaderboardViewController 
@end

@implementation GameCenterLeaderboardViewController
@end

static GameCenterLeaderboardViewController *leaderboard_view_controller = nil;

@interface GameCenterDelegate: NSObject<GKLeaderboardViewControllerDelegate>
- (void)leaderboardViewControllerDidFinish:(GKLeaderboardViewController *)viewController;
@end

static GameCenterDelegate *gamecenter_delegate = nil;

@implementation GameCenterDelegate
- (void)leaderboardViewControllerDidFinish:(GKLeaderboardViewController *)viewController {
    UIViewController *vc = ltIOSGetViewController();
    [vc dismissModalViewControllerAnimated:YES];
}
@end

@interface GameCenterSubmitter : NSObject
+(void)submitScore:(id)data;
@end

@implementation GameCenterSubmitter
+(void)submitScore:(id)data{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    int score = [[data objectForKey:@"score"] intValue];
    NSString* leaderboardCategory = [data objectForKey:@"category"];
    GKScore *scoreReporter = [[[GKScore alloc] initWithCategory:leaderboardCategory] autorelease];
    scoreReporter.value = score;
    [scoreReporter reportScoreWithCompletionHandler:^(NSError *error)
       {
           if (error) {
                NSLog(@"Game Center Score Error: %@", [error localizedDescription]);
           }
       }
    ];
    [data release];
    [pool release];
}
@end

void ltIOSInitGameCenter() {
    #ifdef LTGAMECENTER
    // Check for presence of GKLocalPlayer API.
    Class gcClass = (NSClassFromString(@"GKLocalPlayer"));
 
    // The device must be running running iOS 4.1 or later.
    NSString *reqSysVer = @"4.1";
    NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
    BOOL osVersionSupported = ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending);
 
    if (!(gcClass && osVersionSupported)) {
        return;
    }

    game_center_supported = true;

    [[GKLocalPlayer localPlayer] authenticateWithCompletionHandler:^(NSError *error) {
        if (error == nil) {
            game_center_available = true;
            ltLuaGameCenterBecameAvailable();
        } else {
            game_center_available = false;
        }
    }];

    gamecenter_delegate = [[GameCenterDelegate alloc] init];
    [gamecenter_delegate retain];
    #endif
}

void ltIOSTeardownGameCenter() {
    #ifdef LTGAMECENTER
    //fprintf(stderr, "gamecenter_delegate ref_count = %d\n", [gamecenter_delegate retain_count]);
    [gamecenter_delegate release];
    if (leaderboard_view_controller != nil) {
        //fprintf(stderr, "leaderboard_view_controller ref_count = %d\n", [leaderboard_view_controller retain_count]);
        [leaderboard_view_controller release];
    }
    #endif
}

void ltIOSSubmitGameCenterScore(int score, const char *leaderboard) {
    #ifdef LTGAMECENTER
    if (game_center_available) {
        NSDictionary* data = [[NSDictionary alloc]
            initWithObjectsAndKeys:[NSNumber numberWithInt:score], @"score",
            [NSString stringWithUTF8String:leaderboard], @"category", nil];
        [NSThread detachNewThreadSelector:@selector(submitScore:) toTarget:[GameCenterSubmitter class] withObject:data];
    }
    #endif
}

void ltIOSShowGameCenterLeaderBoard(const char *leaderboard) {
    #ifdef LTGAMECENTER
    if (game_center_available) {
        NSString *category = [NSString stringWithUTF8String:leaderboard];
        if (leaderboard_view_controller == nil) {
            leaderboard_view_controller = [[GameCenterLeaderboardViewController alloc] init];
            [leaderboard_view_controller retain];
        }
        UIViewController *vc = ltIOSGetViewController();
        leaderboard_view_controller.leaderboardDelegate = gamecenter_delegate;
        leaderboard_view_controller.category = category;
        [vc presentModalViewController: leaderboard_view_controller animated: YES];
    }
    #endif
}

bool ltIOSGameCenterIsAvailable() {
    return game_center_available;
}


#endif
