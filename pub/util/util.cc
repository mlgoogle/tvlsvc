// Copyright (c) 2016 The tourism Authors. All rights reserved.
// util.cc
// Created on: 2016年8月15日.
// Author: Paco.
#include "pub/util/util.h"

#include <string.h>
#include <sys/socket.h>
#include <cmath>

namespace util {
std::string GetStrftime(char* format, time_t t) {
  char buf[100];
  memset(buf, 0, sizeof(t));
  strftime(buf, sizeof(buf), format, localtime(&t));
  return buf;
}

int SendFull(int socket, const char *buffer,
            size_t nbytes) {
  ssize_t amt = 0;
  ssize_t total = 0;
  const char *buf = buffer;
  do {
    amt = nbytes;
    amt = send(socket, buf, amt, 0);
    buf = buf + amt;
    nbytes -= amt;
    total += amt;
  } while (amt != -1 && nbytes > 0);
  return amt == -1 ? amt : total;
}

double Angle2Radian(double angle) {
  return angle * PI / 180.0;
}

double CalcDistance(double lon1, double lat1, double lon2, double lat2) {

  double lat_a = Angle2Radian(lat1);
  double lon_a = Angle2Radian(lon1);
  double lat_b = Angle2Radian(lat2);
  double lon_b = Angle2Radian(lon2);
  return acos(
      sin(lat_a) * sin(lat_b) +
      cos(lat_a) * cos(lat_b) * cos(lon_a - lon_b)) * EARTH_R;
}

void BonderOfCoordinate(double lon, double lat, double dis, double* out) {
  lat = Angle2Radian(lat);
  lon = Angle2Radian(lon); //先换算成弧度
  double rad_dist = dis / EARTH_R;  //计算X公里在地球圆周上的弧度
  double lat_min = lat - rad_dist;
  double lat_max = lat + rad_dist;   //计算纬度范围

  double lon_min, lon_max;
  //因为纬度在-90度到90度之间，如果超过这个范围，按情况进行赋值
  if(lat_min > -PI/2 && lat_max < PI/2){
    //开始计算经度范围
    double lon_t = asin( sin(rad_dist) / cos(lat) );
    lon_min = lon - lon_t;
    //同理，经度的范围在-180度到180度之间
    if ( lon_min < -PI ) lon_min += 2 * PI;
    lon_max = lon + lon_t;
    if ( lon_max > PI) lon_max -= 2 * PI;
  }
  else {
    lat_min = lat_min > -PI/2 ? lat_min : -PI/2; // max (lat_min , -PI/2)
    lat_max = lat_max < PI/2 ? lat_max : PI/2; //min (lat_max, PI/2)
    lon_min = -PI;
    lon_max = PI;
  }
  //最后置换成角度进行输出
  out[0] = lon_min * 180 / PI;;
  out[1] = lon_max *180 / PI;
  out[2] = lat_min * 180 / PI;
  out[3] = lat_max * 180 / PI;
}

}  // namespace util

