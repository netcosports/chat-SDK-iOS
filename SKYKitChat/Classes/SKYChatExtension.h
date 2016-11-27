//
//  SKYContainer+Chat.h
//  SKYKit
//
//  Copyright 2016 Oursky Ltd.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.

#import <SKYKit/SKYKit.h>

typedef NS_ENUM(int, SKYChatMetaDataType) {
    SKYChatMetaDataImage,
    SKYChatMetaDataVoice,
    SKYChatMetaDataText
};

extern NSString *_Nonnull const SKYChatMessageUnreadCountKey;
extern NSString *_Nonnull const SKYChatConversationUnreadCountKey;

extern NSString *_Nonnull const SKYChatAdminsMetadataKey;
extern NSString *_Nonnull const SKYChatDistinctByParticipantsMetadataKey;

extern NSString *_Nonnull const SKYChatMetaDataAssetNameImage;
extern NSString *_Nonnull const SKYChatMetaDataAssetNameVoice;
extern NSString *_Nonnull const SKYChatMetaDataAssetNameText;

@class SKYConversation, SKYMessage, SKYUserChannel, SKYUserConversation, SKYChatReceipt;

@interface SKYChatExtension : NSObject

typedef void (^SKYChatUserConversationCompletion)(SKYUserConversation *_Nullable conversation,
                                                  NSError *_Nullable error);
typedef void (^SKYChatMessageCompletion)(SKYMessage *_Nullable message, NSError *_Nullable error);
typedef void (^SKYChatUnreadCountCompletion)(
    NSDictionary<NSString *, NSNumber *> *_Nullable response, NSError *_Nullable error);
typedef void (^SKYChatChannelCompletion)(SKYUserChannel *_Nullable userChannel,
                                         NSError *_Nullable error);
typedef void (^SKYChatGetUserConversationListCompletion)(
    NSArray<SKYUserConversation *> *_Nullable conversationList, NSError *_Nullable error);
typedef void (^SKYChatGetMessagesListCompletion)(NSArray<SKYMessage *> *_Nullable messageList,
                                                 NSError *_Nullable error);
typedef void (^SKYChatGetAssetsListCompletion)(SKYAsset *_Nullable assets,
                                               NSError *_Nullable error);
typedef void (^SKYChatConversationCompletion)(SKYConversation *_Nullable conversation,
                                              NSError *_Nullable error);

/**
 Gets an instance of SKYContainer used by this SKYChatExtension.
 */
@property (strong, nonatomic, readonly, nonnull) SKYContainer *container;

/**
 Creates an instance of SKYChatExtension.

 For most user of the chat extension, get an instance of SKYChatExtension by using the category
 method called `-[SKYContainer chatExtension]`.

 @param container the SKYContainer that contains user credentials and server configuration
 @return an instance of SKYChatExtension
 */
- (nullable instancetype)initWithContainer:(nonnull SKYContainer *)container;

#pragma mark - Conversations

/**
 Creates a conversation with the selected participants.

 All participants will also become the admins of the created conversation. The conversation
 will not be distinct by participants by default.

 If participants list do not include the current user, the current user will be added to the
 list as well. The same apply for admins list.

 @param participantIDs an array of all participants in the conversation
 @param title title of the conversation
 @param metadata application metadata for the conversation
 @param completion completion block
 */
- (void)createConversationWithParticipantIDs:(NSArray<NSString *> *_Nonnull)participantIDs
                                       title:(NSString *_Nullable)title
                                    metadata:(NSDictionary<NSString *, id> *_Nullable)metadata
                                  completion:(SKYChatConversationCompletion _Nullable)completion
    NS_SWIFT_NAME(createConversation(participantIDs:title:metadata:completion:));

/**
 Creates a conversation with the selected participants.

 If participants list do not include the current user, the current user will be added to the
 list as well. The same apply for admins list. If the admins list is not specified, the admins
 list will be the same as the participants list.

 If distinctByParticipants is set to YES, the chat extension will attempt to find an existing
 conversation with the same list of participants before creating a new one.

 @param participantIDs an array of all participants in the conversation
 @param title title of the conversation
 @param metadata application metadata for the conversation
 @param adminIDs an array of all participants that can administrate the conversation
 @param completion completion block
 */
- (void)createConversationWithParticipantIDs:(NSArray<NSString *> *_Nonnull)participantIDs
                                       title:(NSString *_Nullable)title
                                    metadata:(NSDictionary<NSString *, id> *_Nullable)metadata
                                    adminIDs:(NSArray<NSString *> *_Nullable)adminIDs
                      distinctByParticipants:(BOOL)distinctByParticipants
                                  completion:(SKYChatConversationCompletion _Nullable)completion
    NS_SWIFT_NAME(createConversation(participantIDs:title:metadata:adminIDs:distinctByParticipants:
                                     completion:));

/**
 Creates a direct conversation with a specific user.

 The current user and the specified user will be in the participants list and admins list.

 The new conversation will have distinctByParticipants set to YES. This allows the application
 to reuse an exisitng direct conversation.

 @param userID the ID of the other user in the direct conversation
 @param title title of the conversation
 @param metadata application metadata for the conversation
 @param completion completion block
 */
- (void)createDirectConversationWithUserID:(NSString *_Nonnull)userID
                                     title:(NSString *_Nullable)title
                                  metadata:(NSDictionary<NSString *, id> *_Nullable)metadata
                                completion:(SKYChatConversationCompletion _Nullable)completion
    NS_SWIFT_NAME(createDirectConversation(userID:title:metadata:completion:));

/**
 Deletes a conversation.

 @param conversation the conversation to be deleted
 @param completion completion block
 */
- (void)deleteConversation:(SKYConversation *_Nonnull)conversation
                completion:(SKYChatConversationCompletion _Nullable)completion
    NS_SWIFT_NAME(deleteConversation(_:completion:));

/**
 Deletes a conversation by ID.

 @param conversationID the ID of the conversation to be deleted
 @param completion completion block
 */
- (void)deleteConversationWithID:(NSString *_Nonnull)conversationID
                      completion:(SKYChatConversationCompletion _Nullable)completion
    NS_SWIFT_NAME(deleteConversation(id:completion:));

/**
 Saves a conversation.

 This method can be used to save a new conversation or update an existing conversation. This
 method does not reuse an existing conversation even if distinctByParticipants is set to YES.

 To create or reuse a conversation, call createConversation... instead.

 @param conversationID the ID of the conversation to be deleted
 @param completion completion block
 */
- (void)saveConversation:(SKYConversation *_Nonnull)conversation
              completion:(SKYChatConversationCompletion _Nullable)completion;
NS_SWIFT_NAME(saveConversation(_ : completion:));

#pragma mark Fetching User Conversations

/**
 Fetches user conversations.

 @param completion completion block
 */
- (void)fetchUserConversationsWithCompletion:
    (SKYChatGetUserConversationListCompletion _Nullable)completion;

/**
 Fetches a user conversation by ID.

 @param conversationId ID of conversation
 @param completion completion block
 */
- (void)fetchUserConversationWithID:(NSString *_Nonnull)conversationId
                         completion:(SKYChatUserConversationCompletion _Nullable)completion
    NS_SWIFT_NAME(fetchUserConversation(id:completion:));

#pragma mark Conversation Memberships

/**
 Adds participants to a conversation.

 The specified user IDs are added to the conversation record as participants. The modified
 conversation will be saved to the server.

 @param userIDs array of participant user ID
 @param conversation conversation record
 @param completion completion block
 */
- (void)addParticipantsWithUserIDs:(NSArray<NSString *> *_Nonnull)userIDs
                    toConversation:(SKYConversation *_Nonnull)conversation
                        completion:(SKYChatConversationCompletion _Nullable)completion
    NS_SWIFT_NAME(addParticipants(userIDs:to:completion:));

/**
 Removes participants from a conversation.

 The specified user IDs are removed from the conversation record as participants. The modified
 conversation will be saved to the server.

 @param userIDs array of participant user ID
 @param conversation conversation record
 @param completion completion block
 */
- (void)removeParticipantsUserWithIDs:(NSArray<NSString *> *_Nonnull)userIDs
                     fromConversation:(SKYConversation *_Nonnull)conversation
                           completion:(SKYChatConversationCompletion _Nullable)completion
    NS_SWIFT_NAME(removeParticipants(userIDs:from:completion:));

/**
 Adds admins to a conversation.

 The specified user IDs are added to the conversation record as admins. The modified
 conversation will be saved to the server.

 @param userIDs array of admin user ID
 @param conversation conversation record
 @param completion completion block
 */
- (void)addAdminsWithUserIDs:(NSArray<NSString *> *_Nonnull)userIDs
              toConversation:(SKYConversation *_Nonnull)conversation
                  completion:(SKYChatConversationCompletion _Nullable)completion
    NS_SWIFT_NAME(addAdmins(userIDs:to:completion:));

/**
 Removes admins to a conversation.

 The specified user IDs are removed from the conversation record as admins. The modified
 conversation will be saved to the server.

 @param userIDs array of admin user ID
 @param conversation conversation record
 @param completion completion block
 */
- (void)removeAdminsWithUserIDs:(NSArray<NSString *> *_Nonnull)userIDs
               fromConversation:(SKYConversation *_Nonnull)conversation
                     completion:(SKYChatConversationCompletion _Nullable)completion
    NS_SWIFT_NAME(removeAdmins(userIDs:from:completion:));

#pragma mark - Messages

/**
 Creates a message in the specified conversation.

 @param conversation conversation object
 @param body message body
 @param metadata application metadata for the conversation
 @param completion completion block
 */
- (void)createMessageWithConversation:(SKYConversation *_Nonnull)conversation
                                 body:(NSString *_Nullable)body
                             metadata:(NSDictionary<NSString *, id> *_Nullable)metadata
                           completion:(SKYChatMessageCompletion _Nullable)completion;
NS_SWIFT_NAME(createMessage(conversation : body : metadata : completion:));

- (void)createMessageWithConversation:(SKYConversation *_Nonnull)conversation
                                 body:(NSString *_Nullable)body
                                image:(UIImage *_Nullable)image
                           completion:(SKYChatMessageCompletion _Nullable)completion;
NS_SWIFT_NAME(createMessage(conversation : body : image : completion:));

- (void)createMessageWithConversation:(SKYConversation *_Nonnull)conversation
                                 body:(NSString *_Nullable)body
                         voiceFileURL:(NSURL *_Nullable)url
                             duration:(float)duration
                           completion:(SKYChatMessageCompletion _Nullable)completion;
NS_SWIFT_NAME(createMessage(conversation : body : voiceFileURL : duration : completion:));

/**
 Adds a message to a conversation.

 The message is modified with the conversation and saved to the server.

 @param message message to add to a conversation
 @param conversation conversation object
 @param completion completion block
 */
- (void)addMessage:(SKYMessage *_Nonnull)message
    toConversation:(SKYConversation *_Nonnull)conversation
        completion:(SKYChatMessageCompletion _Nullable)completion
    NS_SWIFT_NAME(addMessage(_:to:completion:));

/**
 Adds a message and an asset to a conversation.

 The specified asset is uploaded to the server before attaching to the specified message.
 The message is then modified with the conversation and saved to the server.

 @param message mesaage to add to a conversation
 @param asset asset to add to the mssage
 @param conversation conversation object
 @param completion completion block
 */
- (void)addMessage:(SKYMessage *_Nonnull)message
          andAsset:(SKYAsset *_Nullable)asset
    toConversation:(SKYConversation *_Nonnull)conversation
        completion:(SKYChatMessageCompletion _Nullable)completion;
NS_SWIFT_NAME(addMessage(_ : asset : to : completion:));

/**
 Deletes a message.

 @param message message to delete
 @param completion completion block
 */
- (void)deleteMessage:(SKYMessage *_Nonnull)message
           completion:(SKYChatMessageCompletion _Nullable)completion
    NS_SWIFT_NAME(deleteMessage(_:completion:));

/**
 Deletes a messsage by ID.

 @param messageID ID of the message to delete
 @param completion completion block
 */
- (void)deleteMessageWithID:(NSString *_Nonnull)messageID
                 completion:(SKYChatMessageCompletion _Nullable)completion
    NS_SWIFT_NAME(deleteMessage(id:completion:));

/**
 Fetch messages in a conversation.

 @param conversation conversation object
 @param limit the number of messages to fetch
 @param beforeTime only messages before this time is fetched
 @param completion completion block
 */
- (void)fetchMessagesWithConversation:(SKYConversation *_Nonnull)conversation
                                limit:(NSInteger)limit
                           beforeTime:(NSDate *_Nullable)beforeTime
                           completion:(SKYChatGetMessagesListCompletion _Nullable)completion
    NS_SWIFT_NAME(fetchMessages(conversation:limit:beforeTime:completion:));

/**
 Fetch messages in a conversation by ID.

 @param conversationId ID of the conversation
 @param limit the number of messages to fetch
 @param beforeTime only messages before this time is fetched
 @param completion completion block
 */
- (void)fetchMessagesWithConversationID:(NSString *_Nonnull)conversationId
                                  limit:(NSInteger)limit
                             beforeTime:(NSDate *_Nullable)beforeTime
                             completion:(SKYChatGetMessagesListCompletion _Nullable)completion
    NS_SWIFT_NAME(fetchMessages(conversationID:limit:beforeTime:completion:));

#pragma mark Delivery and Read Status

/**
 Marks messages as read.

 @param messages messages to mark
 @param completion completion block
 */
- (void)markReadMessages:(NSArray<SKYMessage *> *_Nonnull)messages
              completion:(void (^_Nullable)(NSError *_Nullable error))completion
    NS_SWIFT_NAME(markReadMessages(_:completion:));

/**
 Marks messages as read.

 @param messageIDs ID of messages to mark
 @param completion completion block
 */
- (void)markReadMessagesWithID:(NSArray<NSString *> *_Nonnull)messageIDs
                    completion:(void (^_Nullable)(NSError *_Nullable error))completion
    NS_SWIFT_NAME(markReadMessages(id:completion:));

/**
 Marks messages as delivered.

 @param messages messages to delivered
 @param completion completion block
 */
- (void)markDeliveredMessages:(NSArray<SKYMessage *> *_Nonnull)messages
                   completion:(void (^_Nullable)(NSError *_Nullable error))completion
    NS_SWIFT_NAME(markDeliveredMessages(_:completion:));

/**
 Marks messages as delivered.

 @param messageIDs ID of messages to delivered
 @param completion completion block
 */
- (void)markDeliveredMessagesWithID:(NSArray<NSString *> *_Nonnull)messageIDs
                         completion:(void (^_Nullable)(NSError *_Nullable error))completion
    NS_SWIFT_NAME(markDeliveredMessages(id:completion:));

/**
 Fetch delivery and read receipts of a message.

 @param message the message object
 @param completion completion block
 */
- (void)fetchReceiptsWithMessage:(SKYMessage *_Nonnull)message
                      completion:(void (^_Nullable)(NSArray<SKYChatReceipt *> *_Nullable receipts,
                                                    NSError *_Nullable error))completion
    NS_SWIFT_NAME(fetchReceipts(message:completion:));

#pragma mark Message Markers

/**
 Mark a message as the last read message in a user conversation.

 The last read message affects the last read position of messages in a user conversation. The
 number of unread conversations will change. Calling this method will not affect delivery and
 read receipts.

 @param message the message object
 @param userConversation the user conversation object
 @param completion completion block
 */
- (void)markLastReadMessage:(SKYMessage *_Nonnull)message
         inUserConversation:(SKYUserConversation *_Nonnull)userConversation
                 completion:(SKYChatUserConversationCompletion _Nullable)completion
    NS_SWIFT_NAME(markLastReadMessage(_:in:completion:));

/**
 Fetches unread count of a user conversation

 @param userConversation the user conversation object
 @param completion completion block
 */
- (void)fetchUnreadCountWithUserConversation:(SKYUserConversation *_Nonnull)userConversation
                                  completion:(SKYChatUnreadCountCompletion _Nullable)completion
    NS_SWIFT_NAME(fetchUnreadCount(userConversation:completion:));

/**
 Fetches the total unread count of conversations and messages.

 @param completion completion block
 */
- (void)fetchTotalUnreadCount:(SKYChatUnreadCountCompletion _Nullable)completion
    NS_SWIFT_NAME(fetchTotalUnreadCount(completion:));

- (void)getOrCreateUserChannelCompletionHandler:(SKYChatChannelCompletion _Nullable)completion;

#pragma mark - Subscriptions

- (void)subscribeHandler:(void (^_Nonnull)(NSDictionary<NSString *, id> *_Nonnull))messageHandler;

#pragma mark - Assets

- (void)fetchAssetsByRecordId:(NSString *_Nonnull)recordId
                   completion:(SKYChatGetAssetsListCompletion _Nullable)completion;

@end
