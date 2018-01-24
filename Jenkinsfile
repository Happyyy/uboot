pipeline {
  agent any
  stages {
    stage('build') {
      steps {
        sh 'git branch -a'
        sh 'git checkout -b master remotes/origin/master'
        sh 'git pull'
        sh 'make clean'
        sh 'make sc589-mini_defconfig'
        sh 'make'
      }
    }
    stage('Copy') {
      steps {
        sh 'cp u-boot.ldr /tftpboot'
      }
    }
    stage('success') {
      steps {
        echo 'HELLO WORLD'
      }
    }
  }
}