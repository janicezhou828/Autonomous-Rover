void readUS() {
  for (int i = 0; i < SONAR_NUM; i++) {
    median[i] = sonar[i].ping_median(5); //median of 5 values
    if (i == 1) {
      if (median[i] == 0) {
        cm[i] = 1000;
      }
      else {
        cm[i] = sonar[i].convert_cm(median[i]); //convert reading to cm
      }
    }
    else {
      if (median[i] == 0) {
        cm[i] = 1000;
      }
      else {
        cm[i] = conversion[i][0] * median[i] + conversion[i][1];
      }
    }
  }
}
