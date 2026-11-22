
/*
 * Copyright (C) 2026 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "BuoyancyEngine.hh"

#include <gtest/gtest.h>

#include <sdf/Plugin.hh>

#include "gz/sim/EventManager.hh"
#include "gz/sim/SdfEntityCreator.hh"

#include "helpers/UnitTestUtil.hh"

using namespace gz;
using namespace sim;
using namespace systems;
using namespace test;

/// \brief Test topic name resolution for BuoyancyEngine system
/// \param[in] _sdfString The SDF string to load
/// \param[in] _expectedTopicNames The expected resolved topic names
void TestTopicNames(const std::string &_sdfString,
  const BuoyancyEnginePlugin::TopicNames &_expectedTopicNames)
{
  EntityComponentManager ecm;
  EventManager eventMgr;
  Entity modelEntity;
  sdf::Plugin pluginSdf;

  LoadModelContext(_sdfString, ecm, eventMgr, modelEntity, pluginSdf);

  BuoyancyEnginePlugin plugin;
  plugin.Configure(modelEntity, pluginSdf.Element(), ecm, eventMgr);

  const auto topics = plugin.ResolvedTopicNames();

  EXPECT_EQ(topics.cmdTopic, _expectedTopicNames.cmdTopic);
  EXPECT_EQ(topics.statusTopic, _expectedTopicNames.statusTopic);
}

TEST(BuoyancyEngineTest, AbsoluteTopicNames)
{
  // Incomplete sdf, only used to test topic resolution behavior
  const std::string sdfString = R"(
    <sdf version='1.10'>
      <world name='buoyancy_engine_test_world'>
        <model name='buoyancy_engine' namespace='ns'>
          <link name='base_link'/>
          <plugin name='gz::sim::systems::BuoyancyEngine'
                  filename='gz-sim-buoyancy-engine-system'>
            <link_name>base_link</link_name>
            <topic>/test_cmd</topic>
            <status_topic>/test_status</status_topic>
          </plugin>
        </model>
      </world>
    </sdf>)";

  BuoyancyEnginePlugin::TopicNames expectedTopicNames;
  expectedTopicNames.cmdTopic = "/test_cmd";
  expectedTopicNames.statusTopic = "/test_status";
  TestTopicNames(sdfString, expectedTopicNames);
}

TEST(BuoyancyEngineTest, RelativeTopicNames)
{
  // Incomplete sdf, only used to test topic resolution behavior
  const std::string sdfString = R"(
    <sdf version='1.10'>
      <world name='buoyancy_engine_test_world'>
        <model name='buoyancy_engine' namespace='ns'>
          <link name='base_link'/>
          <plugin name='gz::sim::systems::BuoyancyEngine'
                  filename='gz-sim-buoyancy-engine-system'>
            <link_name>base_link</link_name>
            <topic>test_cmd</topic>
            <status_topic>test_status</status_topic>
          </plugin>
        </model>
      </world>
    </sdf>)";

  BuoyancyEnginePlugin::TopicNames expectedTopicNames;
  expectedTopicNames.cmdTopic = "ns/test_cmd";
  expectedTopicNames.statusTopic = "ns/test_status";
  TestTopicNames(sdfString, expectedTopicNames);
}

TEST(BuoyancyEngineTest, DefaultTopicNames)
{
  // Incomplete sdf, only used to test topic resolution behavior
  const std::string sdfString = R"(
    <sdf version='1.10'>
      <world name='buoyancy_engine_test_world'>
        <model name='buoyancy_engine' namespace='ns'>
          <link name='base_link'/>
          <plugin name='gz::sim::systems::BuoyancyEngine'
                  filename='gz-sim-buoyancy-engine-system'>
            <link_name>base_link</link_name>
          </plugin>
        </model>
      </world>
    </sdf>)";

  BuoyancyEnginePlugin::TopicNames expectedTopicNames;
  expectedTopicNames.cmdTopic = "ns/buoyancy_engine";
  expectedTopicNames.statusTopic = "ns/buoyancy_engine/current_volume";
  TestTopicNames(sdfString, expectedTopicNames);
}

TEST(BuoyancyEngineTest, TopicNamesWithoutNs)
{
  // Verify that explicitly configured topic names remain unchanged when no
  // namespace is specified.
  // Incomplete sdf, only used to test topic resolution behavior
  std::string sdfString = R"(
    <sdf version='1.10'>
      <world name='buoyancy_engine_test_world'>
        <model name='buoyancy_engine'>
          <link name='base_link'/>
          <plugin name='gz::sim::systems::BuoyancyEngine'
                  filename='gz-sim-buoyancy-engine-system'>
            <link_name>base_link</link_name>
            <topic>test_cmd</topic>
            <status_topic>test_status</status_topic>
          </plugin>
        </model>
      </world>
    </sdf>)";

  BuoyancyEnginePlugin::TopicNames expectedTopicNames;
  expectedTopicNames.cmdTopic = "test_cmd";
  expectedTopicNames.statusTopic = "test_status";
  TestTopicNames(sdfString, expectedTopicNames);

  // Verify that the original default topic names are preserved when no
  // namespace is specified.
  // Incomplete sdf, only used to test topic resolution behavior
  sdfString = R"(
    <sdf version='1.10'>
      <world name='buoyancy_engine_test_world'>
        <model name='buoyancy_engine'>
          <link name='base_link'/>
          <plugin name='gz::sim::systems::BuoyancyEngine'
                  filename='gz-sim-buoyancy-engine-system'>
            <link_name>base_link</link_name>
          </plugin>
        </model>
      </world>
    </sdf>)";

  expectedTopicNames.cmdTopic = "buoyancy_engine";
  expectedTopicNames.statusTopic = "buoyancy_engine/current_volume";
  TestTopicNames(sdfString, expectedTopicNames);
}

TEST(BuoyancyEngineTest, LegacyNamespace)
{
  // Verify that the legacy <namespace> element preserves the original
  // topic names when no model namespace is specified.
  // Incomplete sdf, only used to test topic resolution behavior
  std::string sdfString = R"(
    <sdf version='1.10'>
      <world name='buoyancy_engine_test_world'>
        <model name='buoyancy_engine'>
          <link name='base_link'/>
          <plugin name='gz::sim::systems::BuoyancyEngine'
                  filename='gz-sim-buoyancy-engine-system'>
            <link_name>base_link</link_name>
            <namespace>legacy_ns</namespace>
          </plugin>
        </model>
      </world>
    </sdf>)";

  BuoyancyEnginePlugin::TopicNames expectedTopicNames;
  expectedTopicNames.cmdTopic = "/model/legacy_ns/buoyancy_engine";
  expectedTopicNames.statusTopic =
    "/model/legacy_ns/buoyancy_engine/current_volume";
  TestTopicNames(sdfString, expectedTopicNames);

  // Incomplete sdf, only used to test topic resolution behavior
  sdfString = R"(
    <sdf version='1.10'>
      <world name='buoyancy_engine_test_world'>
        <model name='buoyancy_engine'>
          <link name='base_link'/>
          <plugin name='gz::sim::systems::BuoyancyEngine'
                  filename='gz-sim-buoyancy-engine-system'>
            <link_name>base_link</link_name>
            <namespace>legacy_ns</namespace>
            <topic>test_cmd</topic>
            <status_topic>test_status</status_topic>
          </plugin>
        </model>
      </world>
    </sdf>)";

  expectedTopicNames.cmdTopic = "/model/legacy_ns/test_cmd";
  expectedTopicNames.statusTopic = "/model/legacy_ns/test_status";
  TestTopicNames(sdfString, expectedTopicNames);

  // Verify that the model namespace takes priority over the deprecated
  // <namespace> element.
  // Incomplete sdf, only used to test topic resolution behavior
  sdfString = R"(
    <sdf version='1.10'>
      <world name='buoyancy_engine_test_world'>
        <model name='buoyancy_engine' namespace='ns'>
          <link name='base_link'/>
          <plugin name='gz::sim::systems::BuoyancyEngine'
                  filename='gz-sim-buoyancy-engine-system'>
            <link_name>base_link</link_name>
            <namespace>legacy_ns</namespace>
          </plugin>
        </model>
      </world>
    </sdf>)";

  expectedTopicNames.cmdTopic = "ns/buoyancy_engine";
  expectedTopicNames.statusTopic = "ns/buoyancy_engine/current_volume";
  TestTopicNames(sdfString, expectedTopicNames);

  // Verify that relative custom topics also use the model namespace
  // when both namespace configurations are present.
  // Incomplete sdf, only used to test topic resolution behavior
  sdfString = R"(
    <sdf version='1.10'>
      <world name='buoyancy_engine_test_world'>
        <model name='buoyancy_engine' namespace='ns'>
          <link name='base_link'/>
          <plugin name='gz::sim::systems::BuoyancyEngine'
                  filename='gz-sim-buoyancy-engine-system'>
            <link_name>base_link</link_name>
            <namespace>legacy_ns</namespace>
            <topic>test_cmd</topic>
            <status_topic>test_status</status_topic>
          </plugin>
        </model>
      </world>
    </sdf>)";

  expectedTopicNames.cmdTopic = "ns/test_cmd";
  expectedTopicNames.statusTopic = "ns/test_status";
  TestTopicNames(sdfString, expectedTopicNames);
}
