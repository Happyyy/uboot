pipeline {
  agent any
  stages {
    stage('build') {
      steps {
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