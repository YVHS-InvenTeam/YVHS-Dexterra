# YVHS 2025 Dexterra Project

<!-- ALL-SOCIAL LINKS:START -->
[![Follow on Instagram](https://img.shields.io/badge/Instagram-E4405F?style=for-the-badge&logo=instagram&logoColor=purple)](https://www.instagram.com/yvhs_inventeam?igsh=MzRlODBiNWFlZA==)
<!-- ALL-SOCIAL LINKS:END -->

<!-- PROJECT DESCRIPTION: START-->
Dexterra is a project made by the Ygnacio Valley High School: InvenTeam, a group of students participating in the year-long Lemelson-MIT InvenTeam initiative
<!-- PROJECT DESCRIPTION: END-->

## Contributors

<!-- ALL-CONTRIBUTORS-LIST:START - Do not remove or modify this section -->
<!-- prettier-ignore-start -->
<!-- markdownlint-disable -->
<table>
  <tbody>
    <tr>
      <a href="https://github.com/Jhony279"><img src="https://avatars.githubusercontent.com/u/114624753?v=4?s=100" width="80px;" alt="Jhony279"><a href="https://github.com/Johnathan Perez/YVHS-Dexterra/commits?author=Jhony279">
    </tr>
  </tbody>
</table>
<!-- markdownlint-restore -->
<!-- prettier-ignore-end -->
<!-- ALL-CONTRIBUTORS-LIST:END -->

## Known issues
* The ESP32-S3 does not have enough GPIO pins to make all 5 motors run (Fixable with an [expansion board](https://www.amazon.com/dp/B08DFNR2JW/?coliid=IAGTPAUPRJLG7&colid=2CQ9CG8OLE4OD&psc=1&ref_=list_c_wl_lv_ov_lig_dp_it))
* Trying to run a 4th motor ends up failing despitie swapping motor controller (Might be battery or software issue)
* Current motors have different torque ratings making movement speed uneven (Recommended: Use [30 rpm N20 motors](https://www.amazon.com/dp/B0DB285Z9J/?coliid=I2LRMXQZMDVLC5&colid=2CQ9CG8OLE4OD&ref_=list_c_wl_lv_ov_lig_dp_it&th=1))

## Getting started
1. Install code locally
2. Open the `Mit4.ino` file in [`Arduino IDE`](https://www.arduino.cc/en/software)
3. Connect to an ESP32-S3 and click upload (Note: must use an ESP32 or else code will execute with problems)

## Resources
* To learn what GPIO's are and how to use them reference [this website](https://randomnerdtutorials.com/esp32-pinout-reference-gpios/)