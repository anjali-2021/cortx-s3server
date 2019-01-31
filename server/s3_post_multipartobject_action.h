/*
 * COPYRIGHT 2016 SEAGATE LLC
 *
 * THIS DRAWING/DOCUMENT, ITS SPECIFICATIONS, AND THE DATA CONTAINED
 * HEREIN, ARE THE EXCLUSIVE PROPERTY OF SEAGATE TECHNOLOGY
 * LIMITED, ISSUED IN STRICT CONFIDENCE AND SHALL NOT, WITHOUT
 * THE PRIOR WRITTEN PERMISSION OF SEAGATE TECHNOLOGY LIMITED,
 * BE REPRODUCED, COPIED, OR DISCLOSED TO A THIRD PARTY, OR
 * USED FOR ANY PURPOSE WHATSOEVER, OR STORED IN A RETRIEVAL SYSTEM
 * EXCEPT AS ALLOWED BY THE TERMS OF SEAGATE LICENSES AND AGREEMENTS.
 *
 * YOU SHOULD HAVE RECEIVED A COPY OF SEAGATE'S LICENSE ALONG WITH
 * THIS RELEASE. IF NOT PLEASE CONTACT A SEAGATE REPRESENTATIVE
 * http://www.seagate.com/contact
 *
 * Original author:  Rajesh Nambiar   <rajesh.nambiar@seagate.com>
 * Original creation date: 6-Jan-2016
 */

#pragma once

#ifndef __S3_SERVER_S3_POST_MULTIPARTOBJECT_ACTION_H__
#define __S3_SERVER_S3_POST_MULTIPARTOBJECT_ACTION_H__

#include <gtest/gtest_prod.h>
#include <memory>
#include "s3_action_base.h"
#include "s3_bucket_metadata.h"
#include "s3_clovis_writer.h"
#include "s3_factory.h"
#include "s3_object_metadata.h"
#include "s3_part_metadata.h"
#include "s3_timer.h"
#include "s3_uuid.h"

class S3PostMultipartObjectAction : public S3Action {
  struct m0_uint128 oid;
  int layout_id;
  struct m0_uint128 old_oid;
  int old_layout_id;
  struct m0_uint128 object_list_oid;
  struct m0_uint128 multipart_index_oid;
  short tried_count;
  std::string salt;
  std::shared_ptr<S3BucketMetadata> bucket_metadata;
  std::shared_ptr<S3ObjectMetadata> object_metadata;
  std::shared_ptr<S3ObjectMetadata> object_multipart_metadata;
  std::shared_ptr<S3PartMetadata> part_metadata;
  std::shared_ptr<S3ClovisWriter> clovis_writer;
  std::string upload_id;

  S3Timer create_object_timer;

  std::shared_ptr<S3BucketMetadataFactory> bucket_metadata_factory;
  std::shared_ptr<S3ObjectMetadataFactory> object_metadata_factory;
  std::shared_ptr<S3ObjectMultipartMetadataFactory> object_mp_metadata_factory;
  std::shared_ptr<S3PartMetadataFactory> part_metadata_factory;
  std::shared_ptr<S3ClovisWriterFactory> clovis_writer_factory;
  std::shared_ptr<ClovisAPI> s3_clovis_api;

 protected:
  void set_oid(struct m0_uint128 new_oid) { oid = new_oid; }
  struct m0_uint128 get_oid() {
    return oid;
  }

 public:
  S3PostMultipartObjectAction(
      std::shared_ptr<S3RequestObject> req,
      S3BucketMetadataFactory* bucket_meta_factory = NULL,
      S3ObjectMultipartMetadataFactory* object_mp_meta_factory = NULL,
      S3ObjectMetadataFactory* object_meta_factory = NULL,
      S3PartMetadataFactory* part_meta_factory = NULL,
      S3ClovisWriterFactory* clovis_writer_factory = NULL,
      std::shared_ptr<ClovisAPI> clovis_api = nullptr);

  void setup_steps();

  void fetch_bucket_info();
  void fetch_bucket_info_failed();
  void fetch_object_info();
  void fetch_object_info_status();
  void check_upload_is_inprogress();
  void create_object();
  void create_object_failed();
  virtual void collision_occured();
  void create_new_oid(struct m0_uint128 current_oid);
  void save_upload_metadata();
  void save_upload_metadata_failed();
  void create_part_meta_index();
  void create_part_meta_index_failed();
  void save_multipart_metadata();
  void save_multipart_metadata_failed();
  void send_response_to_s3_client();

  // rollback functions
  void rollback_create();
  void rollback_create_failed();
  void rollback_upload_metadata();
  void rollback_upload_metadata_failed();
  void rollback_create_part_meta_index();
  void rollback_create_part_meta_index_failed();

  std::shared_ptr<S3RequestObject> get_request() { return request; }

  FRIEND_TEST(S3PostMultipartObjectTest, ConstructorTest);
  FRIEND_TEST(S3PostMultipartObjectTest, FetchBucketInfo);
  FRIEND_TEST(S3PostMultipartObjectTest, UploadInProgress);
  FRIEND_TEST(S3PostMultipartObjectTest, CreateObject);
  FRIEND_TEST(S3PostMultipartObjectTest, CreateObjectFailed);
  FRIEND_TEST(S3PostMultipartObjectTest, CreateObjectFailedToLaunch);
  FRIEND_TEST(S3PostMultipartObjectTest, CreateObjectFailedDueToCollision);
  FRIEND_TEST(S3PostMultipartObjectTest, CreateNewOid);
  FRIEND_TEST(S3PostMultipartObjectTest, FetchObjectInfoMultipartPresent);
  FRIEND_TEST(S3PostMultipartObjectTest, FetchObjectInfoMultipartNotPresent);
  FRIEND_TEST(S3PostMultipartObjectTest, FetchObjectInfoObjectNotPresent);
  FRIEND_TEST(S3PostMultipartObjectTest, FetchObjectInfoStatusObjectNotPresent);
  FRIEND_TEST(S3PostMultipartObjectTest, FetchObjectInfoStatusObjectPresent);
  FRIEND_TEST(S3PostMultipartObjectTest, CollisionOccured);
  FRIEND_TEST(S3PostMultipartObjectTest, RollbackCreate);
  FRIEND_TEST(S3PostMultipartObjectTest, RollbackCreateFailedMetadataMissing);
  FRIEND_TEST(S3PostMultipartObjectTest, RollbackCreateFailedMetadataFailed);
  FRIEND_TEST(S3PostMultipartObjectTest, RollbackCreateFailedMetadataFailed1);
  FRIEND_TEST(S3PostMultipartObjectTest,
              RollbackUploadMetadataFailMetadataPresent);
  FRIEND_TEST(S3PostMultipartObjectTest, RollbackPartMetadataIndex);
  FRIEND_TEST(S3PostMultipartObjectTest, SaveUploadMetadata);
  FRIEND_TEST(S3PostMultipartObjectTest, RollbackPartMetadataIndexFailed);
  FRIEND_TEST(S3PostMultipartObjectTest, SaveUploadMetadataFailed);
  FRIEND_TEST(S3PostMultipartObjectTest, SaveUploadMetadataFailedToLaunch);
  FRIEND_TEST(S3PostMultipartObjectTest, RollbackUploadMetadata);
  FRIEND_TEST(S3PostMultipartObjectTest,
              RollbackUploadMetadataFailMetadataMissing);
  FRIEND_TEST(S3PostMultipartObjectTest, CreatePartMetadataIndex);
  FRIEND_TEST(S3PostMultipartObjectTest, SaveMultipartMetadata);
  FRIEND_TEST(S3PostMultipartObjectTest, SaveMultipartMetadataFailed);
  FRIEND_TEST(S3PostMultipartObjectTest, SaveMultipartMetadataFailedToLaunch);
  FRIEND_TEST(S3PostMultipartObjectTest, Send500ResponseToClient);
  FRIEND_TEST(S3PostMultipartObjectTest, Send404ResponseToClient);
  FRIEND_TEST(S3PostMultipartObjectTest, Send200ResponseToClient);
};

#endif
