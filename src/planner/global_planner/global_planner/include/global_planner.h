/***********************************************************
 *
 * @file: global_planner.h
 * @breif: Contains the abstract global planner class
 * @author: Yang Haodong
 * @update: 2022-10-24
 * @version: 2.1
 *
 * Copyright (c) 2023， Yang Haodong
 * All rights reserved.
 * --------------------------------------------------------
 *
 **********************************************************/
#ifndef GLOBAL_PLANNER_H
#define GLOBAL_PLANNER_H

#define LETHAL_COST 253      // lethal cost
#define NEUTRAL_COST 50      // neutral cost
#define OBSTACLE_FACTOR 0.5  // obstacle factor

#include <costmap_2d/cost_values.h>
#include <unordered_set>

#include "nodes.h"

namespace global_planner
{
/**
 * @brief Abstract class that is inherited by concerete implementaions of global planner classes.
 *        The Plan function is a pure virtual funciton that is overloaded
 */
class GlobalPlanner
{
public:
  /**
   * @brief Construct a new Global Planner object
   * @param nx          pixel number in costmap x direction
   * @param ny          pixel number in costmap y direction
   * @param resolution  costmap resolution
   */
  GlobalPlanner(int nx, int ny, double resolution);

  /**
   * @brief Destroy the Global Planner object
   */
  virtual ~GlobalPlanner() = default;

  /**
   * @brief Pure virtual function that is overloadde by planner implementations
   * @param global_costmap global costmap
   * @param start         start node
   * @param goal          goal node
   * @param path          optimal path consists of Node
   * @param expand        containing the node been search during the process
   * @return  true if path found, else false
   */
  virtual bool plan(const unsigned char* global_costmap, const Node& start, const Node& goal, std::vector<Node>& path,
                    std::vector<Node>& expand) = 0;

  /**
   * @brief Set or reset costmap size
   * @param nx  pixel number in costmap x direction
   * @param ny  pixel number in costmap y direction
   */
  void setSize(int nx, int ny);

  /**
   * @brief Set or reset costmap resolution
   * @param resolution  costmap resolution
   */
  void setResolution(double resolution);

  /**
   * @brief Set or reset lethal cost
   * @param lethal_cost lethal cost
   */
  void setLethalCost(unsigned char lethal_cost);

  /**
   * @brief Set or reset neutral cost
   * @param neutral_cost  neutral cost
   */
  void setNeutralCost(unsigned char neutral_cost);

  /**
   * @brief Set or reset obstacle factor
   * @param factor  obstacle factor
   */
  void setFactor(double factor);

  /**
   * @brief Transform from grid map(x, y) to grid index(i)
   * @param x grid map x
   * @param y grid map y
   * @return  index
   */
  int grid2Index(int x, int y);

  /**
   * @brief Transform from grid index(i) to grid map(x, y)
   * @param i grid index i
   * @param x grid map x
   * @param y grid map y
   */
  void index2Grid(int i, int& x, int& y);

  /**
   * @brief Transform from grid map(x, y) to costmap(x, y)
   * @param gx  grid map x
   * @param gy  grid map y
   * @param mx  costmap x
   * @param my  costmap y
   */
  void map2Grid(double mx, double my, int& gx, int& gy);

  /**
   * @brief Transform from costmap(x, y) to grid map(x, y)
   * @param gx grid map x
   * @param gy grid map y
   * @param mx costmap x
   * @param my costmap y
   */
  void grid2Map(int gx, int gy, double& mx, double& my);

  /**
   * @brief Get permissible motion
   * @return  Node vector of permissible motions
   */
  std::vector<Node> getMotion();

  /**
   * @brief Inflate the boundary of costmap into obstacles to prevent cross planning
   * @param costarr costmap pointer
   */
  void outlineMap(unsigned char* costarr);

protected:
  /**
   * @brief Convert closed list to path
   * @param closed_list closed list
   * @param start       start node
   * @param goal        goal node
   * @return  vector containing path nodes
   */
  std::vector<Node> _convertClosedListToPath(std::unordered_set<Node, NodeIdAsHash, compare_coordinates>& closed_list,
                                             const Node& start, const Node& goal);

  // lethal cost and neutral cost
  unsigned char lethal_cost_, neutral_cost_;
  // pixel number in costmap x, y and total
  int nx_, ny_, ns_;
  // costmap resolution
  double resolution_;
  // obstacle factor(greater means obstacles)
  double factor_;
};
}  // namespace global_planner
#endif  // PLANNER_HPP