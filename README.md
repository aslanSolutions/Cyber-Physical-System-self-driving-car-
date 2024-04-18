# Cyber-Physical Systems Project
<div align="center">

  [![pipeline status](https://git.chalmers.se/courses/dit638/students/2024-group-08/badges/main/pipeline.svg)]  
  [![Status](https://img.shields.io/badge/status-active-success.svg)]() 
  [![GitHub Pull Requests](https://img.shields.io/github/issues-pr/kylelobo/The-Documentation-Compendium.svg)](https://git.chalmers.se/courses/dit638/students/2024-group-08/-/branches)
 

</div>

----

## Name
Self-driving car "Cyber-Physical systems"
## Description

The cyber-physical system we are developing serves as a sophisticated 
framework for the autonomous vehicle, featuring intricate integration and 
collaboration between computational and physical components. This system 
constitutes a novel paradigm in vehicular technology, where the 
self-driving car's movement is orchestrated by a confluence of digital and
physical processes.

At its core, the system embodies a robust cyber-physical interface that 
establishes communication channels between the vehicle and its surrounding
environment. This interface encompasses sensors, actuators, and 
communication protocols tailored to the specific needs of self-driving 
vehicles.

Through this comprehensive integration, the system enables the vehicle to 
perceive its surroundings, make real-time decisions, and control its 
movement autonomously. The cyber-physical architecture fosters seamless 
coordination between the vehicle's computational and physical subsystems, 
ensuring optimal performance and safety.

The system's design prioritizes modularity and flexibility, allowing for 
future enhancements and modifications. This adaptability enables us to 
accommodate evolving technological advancements and maintain the system's 
effectiveness over time.


## Badges
<img width="600" hight="400" src="https://htn-expert.com/wp-content/uploads/2023/02/Cyber-Physical-Systems-CPS-HTN-Hansa-Tek-Netics.jpg">

[Online source](https://htn-expert.com/blog/cps/)

## Visuals
Coming soon...

## Prerequisties
- Git
- CMake 
- C++

## Installation
To install and build the project in this project there will be several steps: 

1. Clone the repository
```bash
git clone https://git.chalmers.se/courses/dit638/students/2024-group-08.git
```

2. Open the project directory
```bash
cd 2024-group-08
```
3. Create a folder called build
```bash
mkdir build
```
4. Build the project files
```bash
cmake ..
```
5. Run command make
```bash
make
```
6. Run project test
```bash
make test
```


## Usage
If you follow the installation steps one by one you will have the bellow output:
```bash
Running tests...
Test project /home/mali/Desktop/Cyber System/Assignments/Assignment 5/build
    Start 1: helloworld-Runner
1/1 Test #1: helloworld-Runner ................   Passed    0.01 sec

100% tests passed, 0 tests failed out of 1

Total Test time (real) =   0.01 sec

```

## Team Workflow
when adding new feature, we follow these steps:

1. Create a new branch for the feature using the naming convention: feature/<feature_name>.

    - Where will use the feature branch concept [Link](https://git-scm.com/book/en/v2/Git-Branching-Branching-Workflows)
2. Implement the feature on the feature branch.
3. Regularly commit changes with descriptive commit messages.
    
    - The commit structue will follow this stucture [Link](https://cbea.ms/git-commit/)
4. Once the feature is complete, submit a merge request to merge the feature branch into the main branch.

    - That branch will be Git-based protecting branch [Link](https://docs.gitlab.com/ee/user/project/protected_branches.html)
5. Assign a team member to review the merge request following our code review guidelines.

    - Where we will apply the followed structure [Link](https://phauer.com/2018/code-review-guidelines/)

## Support
We have the support team  will be available 24/7

- Morning time 8:00-20:00:

    - Yousef Abbas: yousefab@chalmers.se "@yousefab""
    - Muhammed Ali Ehasan: elhasan@chalmers.se "@elhasan"
- Evening time 20:00-8:00
    
    - Zakaria Aslan: muhmmad@chalmers.se "@muhmmad"

## Roadmap
1. Requirements Gathering & Analysis (27 March- 07 April):
    
    -  Define the system's functional and non-functional requirements, including real-time performance, safety, and security.
2. System Design and Architecture (08 April- 12 April)

    - Develop the overall system architecture, including the sensor network and control unit.
    - Select the appropriate software, and communication technologies to meet the project's requirements.
3. Control Unit Implementation (14 April- 01 May)

    - Develop the control unit software, including the data processing, analytics, and control algorithms.
    - Integrate the sensor data and implement the monitoring and control functionalities.
4. Integration and Testing (02 May - 10 May)

    - Conduct comprehensive testing to ensure the system's reliability, safety, and performance.
5. Preparing for the final presentaion (11 May - ...)

    - Write the system's conclusion.
    - Prepare the final presentation.

## Contributing
We welcome contributions from the community to help enhance and improve our cyber-physical system for autonomous vehicles. To get started, please follow these steps:

### Getting Started
1. Fork the repository and clone it to your local machine.
2. Set up the development environment as described in the [README.md](readme.md) file.
3. Create a new branch for your feature or bug fix.

### Development Guidelines
1. Ensure your code follows the project's coding style and conventions.
2. Write unit tests and integration tests to verify your changes.
3. Update the relevant documentation, including the [README.md](readme.md) file.
4. Use clear and descriptive commit messages.

### Submitting Changes
1. Push your branch to your forked repository.
2. Create a new pull request (PR) from your forked repository to the main project repository.
3. Provide a detailed description of your changes and reference any related issues or discussions.

### Code Review and Merging
    Your pull request will be reviewed by the project maintainers. Once approved, your changes will be merged into the main codebase.

Our project uses a continuous integration (CI) pipeline to ensure the quality and stability of the codebase. The CI system will automatically run the test suite and handle the deployment of the updated system.

## Authors and acknowledgment
We would like to express our sincere appreciation to Professor Christian Berger, who has been instrumental in guiding us throughout this project. His continuous support, feedback, and the learning sessions he has provided have been invaluable in helping us achieve the project's outcomes.

Project developers: 

1. Muhammed Ali Elhasan
2. Yousef Abbas
3. Zakaria Aslan

## License
- This project is released under the terms of the MIT License- [![License](https://img.shields.io/badge/License-MIT-BLUE.svg)](https://opensource.org/licenses/MIT)

