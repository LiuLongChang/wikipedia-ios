
#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef void (^ WMFCaptchaHandler)(NSURL* captchaURL);

/**
 *  This class manages all aspects of authentication.
 *
 *  It abstracts and encapsulates the other fetchers and model classes to provide a
 *  simple interface for performing authentication tasks.
 *
 *  NOTE: This class is NOT STATELESS. Only one authentication operation shoud be running at a given time
 *  The class will return an error if a second operation is requested while one is in progress
 *  This behavior will be resolved when the T124408 is completed
 */
@interface WMFAuthenticationManager : NSObject

/**
 *  The current logged in user. If nil, no user is logged in
 */
@property (strong, nonatomic, readonly, nullable) NSString* loggedInUsername;

/**
 *  Returns YES if a user is logged in, NO otherwise
 */
@property (assign, nonatomic, readonly) BOOL isLoggedIn;

/**
 *  Get the shared instance of this class
 *
 *  @return The shared Authentication Manager
 */
+ (instancetype)sharedInstance;

/**
 *  Get a captcha for account creation. This operation will also check if the username/password is valid.
 *
 *  @param username The username to create
 *  @param password The password for the new user
 *  @param email    The email for the new user
 *  @param captcha  The handler for the returned captcha URL
 *  @param failure  The handler for any errors
 */
- (void)getAccountCreationCaptchaWithUsername:(NSString*)username password:(NSString*)password email:(nullable NSString*)email captcha:(WMFCaptchaHandler)captcha failure:(WMFErrorHandler)failure;

/**
 *  Create an account for the username in the above method passing in the answer to the captcha.
 *  If the answer is incorrect, the captcha block will be invoked with a new captcha.
 *  After getting a new answer from the user, simply invoke this method again.
 *
 *  @param captchaText The answer text for the given captcha
 *  @param success     The handler for success - at this point the user is logged in
 *  @param captcha     The handler for the returned captcha URL
 *  @param failure     The handler for any errors
 */
- (void)createAccountWithCaptchaText:(NSString*)captchaText success:(dispatch_block_t)success captcha:(WMFCaptchaHandler)captcha failure:(WMFErrorHandler)failure;

/**
 *  Login with the given username and password
 *
 *  @param username The username to authenticate
 *  @param password The password for the user
 *  @param success  The handler for success - at this point the user is logged in
 *  @param failure     The handler for any errors
 */
- (void)loginWithUsername:(NSString*)username password:(NSString*)password success:(nullable dispatch_block_t)success failure:(nullable WMFErrorHandler)failure;

/**
 *  Logs in a user using saved credentials in the keychain
 *
 *  @param success  The handler for success - at this point the user is logged in
 *  @param failure     The handler for any errors
 */
- (void)loginWithSavedCredentialsWithSuccess:(nullable dispatch_block_t)success failure:(nullable WMFErrorHandler)failure;

/**
 *  Logs out any authenticated user and clears out any associated cookies
 */
- (void)logout;


@end

NS_ASSUME_NONNULL_END