#pragma once

#define DRAW_DEBUGSPHERE(Location) if(GetWorld()){DrawDebugSphere(GetWorld(), Location, 50.f, 24, FColor::Cyan, false, 30.f);}
#define DRAW_DEBUGSPHERECOLOR(Location, Color) if(GetWorld()){DrawDebugSphere(GetWorld(), Location, 50.f, 24, Color, false, 5.f);}
#define DRAW_DEBUGSPHERE_SINGLEFRAME(Location) if(GetWorld()){DrawDebugSphere(GetWorld(), Location, 50.f, 24, FColor::Cyan, false, -1.f);}

#define DRAW_DEBUGLINE(StartLocation, EndLocation) if(GetWorld()){DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 30.f, 0, 1.f);}
#define DRAW_DEBUGLINE_SINGLEFRAME(StartLocation, EndLocation) if(GetWorld()){DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1.f, 0, 1.f);}

#define DRAW_DEBUGPOINT(Location) if(GetWorld()){DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Red, false, 30.f, 0);}
#define DRAW_DEBUGPOINT_SINGLEFRAME(Location) if(GetWorld()){DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Red, false, -1.f, 0);}

#define DRAW_DEBUGVECTOR(StratPoint, EndPoint) if(GetWorld())\
  {\
      DrawDebugLine(GetWorld(), StratPoint, EndPoint, FColor::Red, false, 30.f, 0, 1.f);\
      DrawDebugPoint(GetWorld(), StratPoint, 15.f, FColor::Red, false, 30.f, 0);\
  }
#define DRAW_DEBUGVECTOR_SINGLEFRAME(StratPoint, EndPoint) if(GetWorld())\
  {\
      DrawDebugLine(GetWorld(), StratPoint, EndPoint, FColor::Red, false, -1.f, 0, 1.f);\
      DrawDebugPoint(GetWorld(), StratPoint, 15.f, FColor::Red, false, -1.f, 0);\
  }