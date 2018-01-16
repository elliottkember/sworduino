namespace FlameCrystallization {

  CRGB flameColor, flameBackgroundColor;
  CRGB BlackColor = CRGB::Black;
  int flame_crystallization_iterations = 0;

  void reset() {
    flame_crystallization_iterations = 0;
  }

  CRGB RandomColor() {
    return CHSV( Util::rand_range(0,255), 255, 255);
  }

  // In radians
  float HueDistance(float hue1, float hue2) {
    return std::min(fabs(hue1-hue2), 2.0*PI-fabs(hue1-hue2));
  }

  float AtanHueFromCRGB(CRGB c) {
    float r, g, b;
    r = (float)c.r/255.0 * 2 * PI;
    g = (float)c.g/255.0 * 2 * PI;
    b = (float)c.b/255.0 * 2 * PI;
    
    float intermediate = atan2(pow(3.0,0.5) * (g-b), 2.0*r-g-b);
    
    // Keep values within the range 0-2*PI
    return (intermediate > 0 ? intermediate : (2*PI + intermediate));
  }

  float ColorDistance(CRGB c1, CRGB c2) {
    return pow(pow(c1.red/255.0-c2.red/255.0, 2.0) + pow(c1.green/255.0-c2.green/255.0, 2.0) + pow(c1.blue/255.0-c2.blue/255.0, 2.0), 0.5);
  }
  
  CRGB RandomColorExcluding(CRGB oldColor) {
    float hueOld = AtanHueFromCRGB(oldColor);
    
    CRGB newColor;
    for (;;) {
      newColor = RandomColor();
  
      float hueNew= AtanHueFromCRGB(newColor);
  
      float hueDistance = HueDistance(hueOld, hueNew);
    
      if((newColor!=oldColor) 
      && (ColorDistance(newColor, oldColor)>1.2) 
      && ( hueDistance > PI/3.0 )
      ){
        return newColor;
      }
    }
  }
  
  bool flame_crystallization() {
    // Set the flame and background colors on first iteration
    // or after the previous flame fully engulfed the light stick.
    if (flame_crystallization_iterations == 0) {
      flameColor = RandomColor();
      if(flameBackgroundColor == BlackColor) {
        flameBackgroundColor = RandomColorExcluding(flameColor);
      } else {
        flameColor = RandomColorExcluding(flameBackgroundColor);
      }
  
      float dist = ColorDistance(flameBackgroundColor, flameColor);
      float hueBG = AtanHueFromCRGB(flameBackgroundColor);
      float hueF = AtanHueFromCRGB(flameColor);
      float hueDistance = HueDistance(hueF, hueBG);

      if(DEBUG) {
        Serial.println(String("Hues are ") + String(hueF) + String(" (flame) and ") + String(hueBG) + String(" (bg). Color distance is ") + String(dist) + String(" and hue distance is ") + String(hueDistance));
      }
    }
  
    // Build the flame
    for (int i=0; i<N_CELLS; i++) {
      struct Grid::Point pt = Grid::GetPoint(i);
  
      if (flame_crystallization_iterations == 0) {
        if (IsBottomRow(pt)) {
          Soulmate::led_arr[i] = flameColor;
        } else {
          Soulmate::led_arr[i] = flameBackgroundColor;
        }
  
        if(DEBUG){
          int retrieved = GetIndex(pt);
          Serial.printf("orig: %d => (%d, %d) => retrieved %d\n", i, pt.x, pt.y, retrieved);
        }
        
      } else {
        // (The base always stays on fire so skip y==0)
        if (!IsBottomRow(pt)) {
  
          // Skip the point if it's already on fire
          if(Soulmate::led_arr[GetIndex(pt)] == flameColor) {
            continue;
          }
  
          struct Grid::Neighbors nb = Grid::GetNeighbors(pt);
          
          int burningNeighbors = 0;
          int skippedNeighbors = 0;
          for(int j=0; j<8; j++) {
            struct Grid::Point npt = nb.v[j];
  
            if(DEBUG){
              Serial.printf("%d, %d\n", npt.x, npt.y);
            }
  
            // Skip invalid points
            if (npt.x==-1 || npt.y == -1) {
              if(DEBUG){
                Serial.println("NEGATIVE VALUE DETECTED");
              }
              skippedNeighbors++;
              continue;
            }
  
            // Count burning neighbors
            if(Soulmate::led_arr[GetIndex(npt)] == flameColor){
              burningNeighbors++;
            }
          }
  
          if(burningNeighbors > 0) {
            burningNeighbors += skippedNeighbors;
          }
  
          // Set the burners
          if(burningNeighbors>=7){
            Soulmate::led_arr[GetIndex(pt)] = flameColor;
          }else if((burningNeighbors>=1) && (Util::rand_range(0, 20*burningNeighbors)==0)){
            Soulmate::led_arr[GetIndex(pt)] = flameColor;
          } else if(burningNeighbors>0) {
            // If not set to a burner, but has burning neighbors, set to an intermediate color
            Soulmate::led_arr[GetIndex(pt)] = blend(flameBackgroundColor, flameColor, 255.0*((float)burningNeighbors/13.0));
          }
        }
      }
    }
  
    flame_crystallization_iterations++;
  
    // If every cell is identical, reset
    // Keep the flame color as the new color
    for (int i=(N_CELLS-1); i>0; i--) {
      if (Soulmate::led_arr[i] != Soulmate::led_arr[0]) {
        if(DEBUG){
          Serial.printf("cell %d is different from cell %d\n", i, 0);
        }
        return false; // Game is ongoing, not all cells are identical yet
      }
    }
    
    flameBackgroundColor = flameColor;
    flame_crystallization_iterations = 0;
    return true; // Done with a game
  }
}

bool flame_crystallization() {
  return FlameCrystallization::flame_crystallization();
}
