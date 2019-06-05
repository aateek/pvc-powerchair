import subprocess, os

from paver.easy import *
from paver.setuputils import setup

setup(
    name="PVC Chair",
    packages=['pvc'],
    version="0.1.0",
    url="http://robomakery.com",
    author="Dylan Vaughn",
    author_email="dylan@robomakery.com"
)

@task
def docker_build():
    """Builds the docker container"""
    os.system("docker build -t pvc .")

@task
def docker_run():
    """Runs the docker container"""
    os.system("docker run -p 11311:11311 -it pvc")

@task
def docker_rm_containers():
    """Removes all containers"""
    os.system("docker rm $(docker ps -a -q)")

@task
def docker_rm_images():
    """Removes all images"""
    os.system("docker rmi $(docker images -q)")

@task
def ros_rviz():
    """Runs rviz"""
    os.system("sh ./ros/devel/setup.sh && roslaunch pvcchair_description pvcchair_rviz.launch")
    pipe = subprocess.Popen(". ./ros/devel/setup.bash && roslaunch pvcchair_description pvcchair_rviz.launch", stdout=subprocess.PIPE, shell=True)
    output = pipe.communicate()[0]
