# Basilisk Project: Sessile Drop Simulation and Machine Learning

This project combines fluid dynamics simulations using Basilisk with machine learning to model sessile drop behavior.

## Table of Contents
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Simulation Setup](#simulation-setup)
- [Running Simulations](#running-simulations)
- [Machine Learning Training](#machine-learning-training)
- [Making Predictions](#making-predictions)
- [Project Structure](#project-structure)

## Prerequisites

### System Requirements
- Linux-based operating system (Ubuntu/Debian recommended)
- Python 3.7+
- C compiler (GCC)
- Basic development tools

### Required Packages for Basilisk
```bash
sudo apt update
sudo apt install darcs make gawk gnuplot imagemagick ffmpeg graphviz valgrind gifsicle pstoedit gfortran swig libpython-dev
```

## Installation

### 1. Install Basilisk

#### Method A: Using darcs (Recommended)
```bash
# Clone the Basilisk repository
darcs clone http://basilisk.fr/basilisk
cd basilisk

# Set up environment variables
echo "export BASILISK=$PWD/src" >> ~/.bashrc
echo 'export PATH=$PATH:$BASILISK' >> ~/.bashrc
source ~/.bashrc

# Compile Basilisk
cd src
ln -s config.gcc config
make
```

#### Method B: Using Tarball
```bash
wget http://basilisk.fr/basilisk/basilisk.tar.gz
tar xzf basilisk.tar.gz
cd basilisk
# Follow same environment setup and compilation steps as above
```

### 2. Verify Installation
```bash
cd $BASILISK
make test  # Run basic tests to verify installation
```

## Simulation Setup

### 1. Prepare Simulation Directory
Create the necessary directory structure:
```bash
mkdir -p dump field image interface
```

### 2. Compile the Sessile Drop Simulation
```bash
# Compile with OpenMP support
qcc -O2 sessile.c -fopenmp -o drop -lm
```

### 3. Create Run Script
Create `run.sh` with the following content:
```bash
#!/bin/bash

# Create necessary directories
mkdir -p dump field image interface

# Compile the simulation
echo "Compiling sessile drop simulation..."
qcc -O2 sessile.c -fopenmp -o drop -lm

# Run simulation and log output
echo "Starting simulation..."
./drop > output.log 2>&1

echo "Simulation completed. Check output.log for details."
```

Make the script executable:
```bash
chmod +x run.sh
```

### 4. Modify Simulation Parameters
Edit `sessile.c` to change radius and contact angle parameters:

```c
// Look for these parameters in sessile.c and modify as needed:
double R0 = 0.5;        // Initial radius
double theta = 45.0;    // Contact angle in degrees
```

## Running Simulations

### Execute the Simulation
```bash
./run.sh
```

This will:
- Create necessary directories
- Compile the simulation code
- Run the sessile drop simulation
- Generate data points in the output files
- Log all output to `output.log`

### Output Files
After successful execution, check for:
- `output.log` - Simulation log file
- Data files in `dump/`, `field/`, `image/` directories
- Generated data points for machine learning training

## Machine Learning Training

### 1. Prepare Training Data
Download the training data from:
```
https://docs.google.com/spreadsheets/d/1R6o4CYXL7O51_teqOF5ZCuA8moIwlZ-k/edit?usp=sharing&ouid=107276384741516461231&rtpof=true&sd=true
```

Save the Excel file to your Google Drive or local storage.

### 2. Google Colab Setup
Open Google Colab and install required packages:

```python
# --- Install Packages ---
!pip install -q \
    catboost \
    lightgbm \
    pytorch-tabnet \
    torch \
    scikit-learn-intelex \
    tqdm \
    pandas \
    openpyxl \
    xgboost \
    joblib

print("All packages installed! Runtime will restart automatically if needed.\n")
```

### 3. Training Process
1. Upload your Excel data file to Colab
2. Run the training script which will:
   - Load and preprocess the data
   - Train multiple ML models (XGBoost, CatBoost, LightGBM, TabNet)
   - Evaluate model performance
   - Save trained models to your Google Drive

### 4. Model Saving
Ensure the training script includes code to save models:
```python
import joblib

# Save trained models
joblib.dump(model, 'trained_model.pkl')
```

Download the saved models for local use.

## Making Predictions

### 1. Prediction Script
Create a prediction script (`predict.py`) that:
- Loads the trained model
- Takes new diameter and contact angle values as input
- Outputs predictions based on the trained model

## Project Structure
```
project/
├── basilisk/                 # Basilisk installation
├── sessile.c                 # Main simulation code
├── run.sh                    # Simulation runner script
├── output.log               # Simulation output log
├── dump/                    # Simulation dump files
├── field/                   # Field data
├── image/                   # Generated images
├── interface/               # Interface data
├── trained_models/          # Saved ML models
├── training_data.xlsx       # Training dataset
```

## Troubleshooting

### Common Issues

1. **Compilation errors**: Ensure all dependencies are installed and environment variables are set correctly.

2. **Permission denied**: Make sure scripts are executable:
   ```bash
   chmod +x run.sh
   ```

3. **Missing directories**: The run script automatically creates required directories.

4. **Python package conflicts**: Use virtual environments or Colab's clean runtime.

### Getting Help
- Check Basilisk documentation: http://basilisk.fr
- Review simulation output in `output.log`
- Verify all required packages are installed

## Next Steps
1. Run initial simulations to generate data
2. Train ML models with the generated data
3. Use trained models for predictions on new parameters
4. Iterate and refine both simulation parameters and ML models

This setup provides a complete pipeline from fluid dynamics simulation to machine learning prediction for sessile drop behavior analysis.
