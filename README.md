## Usage
* In the docker env
1. `cmake .` (Only need the very first time)
2. `make`
3. `./ChallengeInvis production_ID_4291725.mp4`

## Prompt Answers
* *Suggest the different ways to implement thread pooling and thread priority/scheduling for a RT Linux kernel*
 * To implement thread pooling we could create a vector of threads with a built in function that would continually dequeue jobs from a job queue. The jobs would contain the various image processing tasks
 * The threadpool could then use a mutex to access the latest task and let go of the mutex once it acquired a task.
 * All threads would repeat this process until we were done processing the video and there were no longer any tasks
 * In terms of thread priority there are two main scheduling policies
   * Round Robin: Round Robin is relatively simple and each task gets an equal time slice on the CPU and each task is executed in the order in which they are added to the kernel. This would be fine for a pipeline where each image processing task takes relatively the same amount of time and if the CPU and memory resources are limited.
   * The earliest deadline first pattern might be the most suitable for this type of task. Since each frame needs to come before the following frame all the image processing for a frame has to be done before the program can move on to the next frame. Hence the deadline is very important in this situation.
* *Suggest a testing framework and steps if you were to deploy this code onto on-prem devices (from
development through to prod and deployment)*
  * The very first step for testing would be using extensive unit testing using a framework such as gtest to ensure the code is correct and reliable before it gets merged into development
  * The next step would be to use a CI/CD workflow to automate the building process of the code and ensure the packages are built in a consistent and reproducible way. This could be achieved through the use of docker containers to simulate every environment the code would be built to.
  * Next would be to test the package in a staging environment. In the staging environment we would use regression testing to make sure existing functionality was not affected by new changes and identify and fix any bugs.
  * When deploying to the production environment we would use rolling deployment to minimize downtime and the impact to customers. Additionally, through the use of monitoring tools such as Prometheus we would could quickly detect any errors or failures
  * Finally, we could use a patch management process to apply security fixes and bug fixes to maintain and update the production environment.
* *Suggest possible next steps (related to above) - how could you increase efficiency or do something interesting
with the outputs if you had more time to work on this?*
    * One improvement I was thinking of would be a more sophisticated thread implementation
      * The first improvement would be the implementation of a thread pool to reduce the current problem where two new threads are created in every loop
      * The second improvement would be:
        * The current implementation is bottle necked because it needs for all the image processing on one frame to finish before it can move on to the next
        * One possible improvement we could apply would be to create queues that held the result of each image processing task.
        * When all the queues had at least one processed frame another thread could dequeue an item from every queue and combine them in order to create the final image with all the image processing techniques
        * This way each image processing technique does not have to wait for all the other processing functions to finish before moving on to a new frame
        * Basically this would make the rendering part independent of the processing part.
    * In terms of doing something interesting with the output, in this situation it would be interesting to identify potential empty spots in the warehouse.
      * Using the edge detection and corner detection, we could identify which spots were regularly empty. This would allow warehouse owners to help identify empty space they could better utilize to store more products or identify which of their products were regularly sold.

