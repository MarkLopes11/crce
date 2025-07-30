import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from scipy.integrate import odeint
from scipy.optimize import minimize
from sklearn.metrics import mean_absolute_error, mean_squared_error, r2_score
from sklearn.ensemble import RandomForestRegressor
from sklearn.linear_model import LinearRegression
from sklearn.preprocessing import StandardScaler
from sklearn.model_selection import train_test_split
import networkx as nx
import random
from datetime import datetime, timedelta
import warnings
warnings.filterwarnings('ignore')

class Agent:
    """Individual agent in the disease propagation simulation"""
    
    def __init__(self, agent_id, x=None, y=None):
        self.id = agent_id
        self.x = x if x is not None else random.uniform(0, 100)
        self.y = y if y is not None else random.uniform(0, 100)
        self.state = 'S'  # S: Susceptible, I: Infected, R: Recovered, D: Dead
        self.infection_time = 0
        self.contacts = []
        self.age = random.randint(0, 80)
        self.mobility = random.uniform(0.1, 1.0)
        
    def update_position(self):
        """Update agent position based on mobility"""
        self.x += random.uniform(-self.mobility, self.mobility)
        self.y += random.uniform(-self.mobility, self.mobility)
        self.x = max(0, min(100, self.x))  # Keep within bounds
        self.y = max(0, min(100, self.y))
        
    def get_distance(self, other_agent):
        """Calculate Euclidean distance to another agent"""
        return np.sqrt((self.x - other_agent.x)**2 + (self.y - other_agent.y)**2)

class MultiAgentEpidemicModel:
    """Multi-agent system for disease propagation simulation"""
    
    def __init__(self, population_size=1000, grid_size=100):
        self.population_size = population_size
        self.grid_size = grid_size
        self.agents = []
        self.time_step = 0
        self.history = {'S': [], 'I': [], 'R': [], 'D': []}
        self.contact_network = nx.Graph()
        
        # Disease parameters
        self.infection_rate = 0.3
        self.recovery_rate = 0.1
        self.mortality_rate = 0.02
        self.incubation_period = 5
        self.infectious_period = 10
        self.contact_radius = 3.0
        
        self._initialize_population()
        
    def _initialize_population(self):
        """Initialize agent population"""
        for i in range(self.population_size):
            agent = Agent(i)
            self.agents.append(agent)
            self.contact_network.add_node(i)
            
        # Set initial infected agents (patient zero)
        initial_infected = max(1, self.population_size // 1000)
        for i in range(initial_infected):
            self.agents[i].state = 'I'
            
    def update_contact_network(self):
        """Update contact network based on agent proximity"""
        self.contact_network.clear_edges()
        
        for i, agent1 in enumerate(self.agents):
            for j, agent2 in enumerate(self.agents[i+1:], i+1):
                distance = agent1.get_distance(agent2)
                if distance <= self.contact_radius:
                    self.contact_network.add_edge(i, j, weight=1/distance)
                    
    def transmission_probability(self, infected_agent, susceptible_agent):
        """Calculate transmission probability between two agents"""
        base_prob = self.infection_rate
        
        # Age-based susceptibility (older agents more susceptible)
        age_factor = 1 + (susceptible_agent.age / 100)
        
        # Distance-based transmission
        distance = infected_agent.get_distance(susceptible_agent)
        distance_factor = max(0.1, 1 - (distance / self.contact_radius))
        
        return base_prob * age_factor * distance_factor
        
    def step(self):
        """Execute one time step of the simulation"""
        self.time_step += 1
        
        # Update agent positions
        for agent in self.agents:
            agent.update_position()
            
        # Update contact network
        self.update_contact_network()
        
        # Disease progression
        new_infections = []
        new_recoveries = []
        new_deaths = []
        
        for agent in self.agents:
            if agent.state == 'I':
                agent.infection_time += 1
                
                # Check for recovery or death
                if agent.infection_time >= self.infectious_period:
                    if random.random() < self.mortality_rate:
                        new_deaths.append(agent.id)
                    else:
                        new_recoveries.append(agent.id)
                        
                # Transmission to contacts
                else:
                    neighbors = list(self.contact_network.neighbors(agent.id))
                    for neighbor_id in neighbors:
                        neighbor = self.agents[neighbor_id]
                        if neighbor.state == 'S':
                            trans_prob = self.transmission_probability(agent, neighbor)
                            if random.random() < trans_prob:
                                new_infections.append(neighbor_id)
                                
        # Apply state changes
        for agent_id in new_infections:
            self.agents[agent_id].state = 'I'
            
        for agent_id in new_recoveries:
            self.agents[agent_id].state = 'R'
            
        for agent_id in new_deaths:
            self.agents[agent_id].state = 'D'
            
        # Record history
        counts = {'S': 0, 'I': 0, 'R': 0, 'D': 0}
        for agent in self.agents:
            counts[agent.state] += 1
            
        for state in counts:
            self.history[state].append(counts[state])
            
    def run_simulation(self, max_steps=200):
        """Run the complete simulation"""
        print(f"Running multi-agent simulation with {self.population_size} agents...")
        
        for step in range(max_steps):
            self.step()
            
            # Stop if no more infected agents
            if self.history['I'][-1] == 0 and step > 10:
                break
                
            if step % 20 == 0:
                print(f"Step {step}: S={self.history['S'][-1]}, I={self.history['I'][-1]}, "
                      f"R={self.history['R'][-1]}, D={self.history['D'][-1]}")
                
        print("Simulation completed!")
        return self.history

class SIRModel:
    """Classical SIR epidemiological model"""
    
    def __init__(self, beta=0.3, gamma=0.1, population=1000):
        self.beta = beta  # infection rate
        self.gamma = gamma  # recovery rate
        self.N = population  # total population
        
    def sir_equations(self, y, t):
        """SIR differential equations"""
        S, I, R = y
        dSdt = -self.beta * S * I / self.N
        dIdt = self.beta * S * I / self.N - self.gamma * I
        dRdt = self.gamma * I
        return [dSdt, dIdt, dRdt]
    
    def simulate(self, days=200, initial_infected=1):
        """Simulate SIR model"""
        S0 = self.N - initial_infected
        I0 = initial_infected
        R0 = 0
        
        t = np.linspace(0, days, days)
        solution = odeint(self.sir_equations, [S0, I0, R0], t)
        
        return {
            'time': t,
            'S': solution[:, 0],
            'I': solution[:, 1],
            'R': solution[:, 2]
        }
    
    def calculate_r0(self):
        """Calculate basic reproduction number"""
        return self.beta / self.gamma

class OutbreakPredictor:
    """Machine learning-based outbreak prediction system"""
    
    def __init__(self):
        self.models = {
            'rf': RandomForestRegressor(n_estimators=100, random_state=42),
            'lr': LinearRegression()
        }
        self.scaler = StandardScaler()
        self.trained_models = {}
        
    def create_features(self, data, window_size=7):
        """Create features for ML prediction"""
        features = []
        targets = []
        
        for i in range(window_size, len(data)):
            # Use past window_size days as features
            feature_vector = []
            
            # Historical confirmed cases
            for j in range(window_size):
                feature_vector.append(data.iloc[i-j-1]['Confirmed'])
                
            # Daily new cases
            for j in range(window_size-1):
                new_cases = data.iloc[i-j-1]['Confirmed'] - data.iloc[i-j-2]['Confirmed']
                feature_vector.append(max(0, new_cases))
                
            # Growth rates
            for j in range(1, min(4, window_size)):
                if data.iloc[i-j-1]['Confirmed'] > 0:
                    growth_rate = (data.iloc[i-j]['Confirmed'] / data.iloc[i-j-1]['Confirmed']) - 1
                    feature_vector.append(growth_rate)
                else:
                    feature_vector.append(0)
                    
            features.append(feature_vector)
            targets.append(data.iloc[i]['Confirmed'])
            
        return np.array(features), np.array(targets)
    
    def train_models(self, data):
        """Train all prediction models"""
        print("Training prediction models...")
        
        X, y = self.create_features(data)
        X_scaled = self.scaler.fit_transform(X)
        
        X_train, X_test, y_train, y_test = train_test_split(
            X_scaled, y, test_size=0.2, random_state=42
        )
        
        for name, model in self.models.items():
            print(f"Training {name} model...")
            model.fit(X_train, y_train)
            self.trained_models[name] = model
            
            # Evaluate on test set
            y_pred = model.predict(X_test)
            mae = mean_absolute_error(y_test, y_pred)
            rmse = np.sqrt(mean_squared_error(y_test, y_pred))
            r2 = r2_score(y_test, y_pred)
            
            print(f"{name} - MAE: {mae:.2f}, RMSE: {rmse:.2f}, R²: {r2:.4f}")
            
    def predict(self, data, days_ahead=30, model_name='rf'):
        """Generate predictions for future days"""
        if model_name not in self.trained_models:
            raise ValueError(f"Model {model_name} not trained")
            
        model = self.trained_models[model_name]
        predictions = []
        
        # Use last available data as starting point
        current_data = data.copy()
        
        for day in range(days_ahead):
            X, _ = self.create_features(current_data)
            if len(X) == 0:
                break
                
            X_scaled = self.scaler.transform([X[-1]])
            pred = model.predict(X_scaled)[0]
            predictions.append(max(0, pred))
            
            # Add prediction to data for next iteration
            next_date = pd.to_datetime(current_data.iloc[-1]['Date']) + timedelta(days=1)
            new_row = pd.DataFrame({
                'Date': [next_date.strftime('%Y-%m-%d')],
                'Country': [current_data.iloc[-1]['Country']],
                'Confirmed': [pred],
                'Deaths': [current_data.iloc[-1]['Deaths']],
                'Recovered': [current_data.iloc[-1]['Recovered']]
            })
            current_data = pd.concat([current_data, new_row], ignore_index=True)
            
        return predictions

class EvaluationMetrics:
    """Comprehensive evaluation metrics for outbreak predictions"""
    
    @staticmethod
    def calculate_all_metrics(y_true, y_pred):
        """Calculate comprehensive evaluation metrics"""
        y_true = np.array(y_true)
        y_pred = np.array(y_pred)
        
        # Basic metrics
        mae = mean_absolute_error(y_true, y_pred)
        rmse = np.sqrt(mean_squared_error(y_true, y_pred))
        r2 = r2_score(y_true, y_pred)
        
        # MAPE (Mean Absolute Percentage Error)
        mape = np.mean(np.abs((y_true - y_pred) / np.where(y_true != 0, y_true, 1))) * 100
        
        # SMAPE (Symmetric Mean Absolute Percentage Error)
        smape = np.mean(2 * np.abs(y_true - y_pred) / (np.abs(y_true) + np.abs(y_pred))) * 100
        
        # MBE (Mean Bias Error)
        mbe = np.mean(y_pred - y_true)
        
        # NRMSE (Normalized RMSE)
        nrmse = rmse / (np.max(y_true) - np.min(y_true)) * 100
        
        # Pearson Correlation
        correlation = np.corrcoef(y_true, y_pred)[0, 1]
        
        # Index of Agreement (Willmott's d)
        numerator = np.sum((y_true - y_pred) ** 2)
        denominator = np.sum((np.abs(y_pred - np.mean(y_true)) + np.abs(y_true - np.mean(y_true))) ** 2)
        index_of_agreement = 1 - (numerator / denominator) if denominator != 0 else 0
        
        # Theil's U Statistic
        theil_u = np.sqrt(np.mean((y_pred - y_true) ** 2)) / np.sqrt(np.mean(y_true ** 2))
        
        return {
            'MAE': mae,
            'RMSE': rmse,
            'R²': r2,
            'MAPE': mape,
            'SMAPE': smape,
            'MBE': mbe,
            'NRMSE': nrmse,
            'Correlation': correlation,
            'Index_of_Agreement': index_of_agreement,
            'Theil_U': theil_u
        }
    
    @staticmethod
    def print_metrics(metrics):
        """Print formatted metrics"""
        print("\n" + "="*60)
        print("COMPREHENSIVE EVALUATION METRICS")
        print("="*60)
        
        print(f"Mean Absolute Error (MAE):           {metrics['MAE']:.2f}")
        print(f"Root Mean Square Error (RMSE):       {metrics['RMSE']:.2f}")
        print(f"R-squared (R²):                      {metrics['R²']:.4f}")
        print(f"Mean Absolute Percentage Error:      {metrics['MAPE']:.2f}%")
        print(f"Symmetric MAPE:                      {metrics['SMAPE']:.2f}%")
        print(f"Mean Bias Error (MBE):               {metrics['MBE']:.2f}")
        print(f"Normalized RMSE:                     {metrics['NRMSE']:.2f}%")
        print(f"Pearson Correlation:                 {metrics['Correlation']:.4f}")
        print(f"Index of Agreement:                  {metrics['Index_of_Agreement']:.4f}")
        print(f"Theil's U Statistic:                 {metrics['Theil_U']:.4f}")
        
        # Interpretation
        print("\n" + "-"*60)
        print("PERFORMANCE INTERPRETATION")
        print("-"*60)
        
        # MAPE interpretation
        if metrics['MAPE'] < 10:
            mape_level = "Excellent"
        elif metrics['MAPE'] < 20:
            mape_level = "Good"
        elif metrics['MAPE'] < 50:
            mape_level = "Fair"
        else:
            mape_level = "Poor"
        print(f"Accuracy Level (MAPE):               {mape_level}")
        
        # R² interpretation
        if metrics['R²'] > 0.9:
            r2_level = "Excellent"
        elif metrics['R²'] > 0.7:
            r2_level = "Good"
        elif metrics['R²'] > 0.5:
            r2_level = "Moderate"
        else:
            r2_level = "Poor"
        print(f"Prediction Quality (R²):             {r2_level}")
        
        # Correlation interpretation
        corr = abs(metrics['Correlation'])
        if corr > 0.9:
            corr_level = "Very Strong"
        elif corr > 0.7:
            corr_level = "Strong"
        elif corr > 0.5:
            corr_level = "Moderate"
        elif corr > 0.3:
            corr_level = "Weak"
        else:
            corr_level = "Very Weak"
        print(f"Correlation Strength:                {corr_level}")

def load_covid_data():
    """Load COVID-19 data from the provided URL"""
    try:
        url = "https://raw.githubusercontent.com/datasets/covid-19/main/data/countries-aggregated.csv"
        data = pd.read_csv(url)
        data['Date'] = pd.to_datetime(data['Date'])
        print(f"Loaded {len(data)} records from {data['Country'].nunique()} countries")
        return data
    except Exception as e:
        print(f"Error loading data: {e}")
        return None

def analyze_country_data(data, country='US'):
    """Analyze COVID-19 data for a specific country"""
    country_data = data[data['Country'] == country].copy()
    country_data = country_data.sort_values('Date').reset_index(drop=True)
    
    if len(country_data) == 0:
        print(f"No data found for {country}")
        return None
        
    print(f"\nAnalyzing data for {country}")
    print(f"Data range: {country_data['Date'].min()} to {country_data['Date'].max()}")
    print(f"Total records: {len(country_data)}")
    
    # Calculate daily new cases
    country_data['Daily_New_Cases'] = country_data['Confirmed'].diff().fillna(0)
    country_data['Daily_New_Cases'] = country_data['Daily_New_Cases'].clip(lower=0)
    
    return country_data

def run_complete_analysis():
    """Run complete outbreak prediction analysis"""
    print("🦠 MULTIAGENT INFECTIOUS DISEASE PROPAGATION & OUTBREAK PREDICTION")
    print("="*80)
    
    # 1. Load real COVID-19 data
    print("\n1. Loading COVID-19 dataset...")
    covid_data = load_covid_data()
    if covid_data is None:
        return
    
    # 2. Analyze specific country
    country = 'US'  # Change this to analyze different countries
    country_data = analyze_country_data(covid_data, country)
    if country_data is None:
        return
    
    # 3. Run Multi-Agent Simulation
    print(f"\n2. Running Multi-Agent Epidemic Simulation...")
    ma_model = MultiAgentEpidemicModel(population_size=1000)
    ma_model.infection_rate = 0.3
    ma_model.recovery_rate = 0.1
    ma_history = ma_model.run_simulation(max_steps=150)
    
    # 4. Compare with SIR Model
    print(f"\n3. Running Classical SIR Model...")
    sir_model = SIRModel(beta=0.3, gamma=0.1, population=1000)
    sir_results = sir_model.simulate(days=150)
    r0 = sir_model.calculate_r0()
    print(f"Basic Reproduction Number (R₀): {r0:.2f}")
    
    # 5. Machine Learning Predictions
    print(f"\n4. Training Machine Learning Models...")
    predictor = OutbreakPredictor()
    
    # Use sufficient data for training
    if len(country_data) > 50:
        predictor.train_models(country_data)
        
        # Generate predictions
        predictions = predictor.predict(country_data.iloc[:-30], days_ahead=30)
        actual_values = country_data.iloc[-30:]['Confirmed'].values
        
        # Evaluate predictions
        print(f"\n5. Evaluating Prediction Models...")
        min_length = min(len(predictions), len(actual_values))
        if min_length > 0:
            metrics = EvaluationMetrics.calculate_all_metrics(
                actual_values[:min_length], 
                predictions[:min_length]
            )
            EvaluationMetrics.print_metrics(metrics)
    
    # 6. Visualization
    print(f"\n6. Generating Visualizations...")
    create_visualizations(ma_history, sir_results, country_data, country)
    
    print(f"\n✅ Analysis completed successfully!")

def create_visualizations(ma_history, sir_results, country_data, country):
    """Create comprehensive visualizations"""
    fig, axes = plt.subplots(2, 2, figsize=(15, 12))
    fig.suptitle(f'Multiagent Disease Outbreak Analysis - {country}', fontsize=16)
    
    # Multi-Agent Simulation Results
    ax1 = axes[0, 0]
    time_steps = range(len(ma_history['S']))
    ax1.plot(time_steps, ma_history['S'], label='Susceptible', color='blue')
    ax1.plot(time_steps, ma_history['I'], label='Infected', color='red')
    ax1.plot(time_steps, ma_history['R'], label='Recovered', color='green')
    ax1.plot(time_steps, ma_history['D'], label='Dead', color='black')
    ax1.set_title('Multi-Agent Simulation')
    ax1.set_xlabel('Time Steps')
    ax1.set_ylabel('Population')
    ax1.legend()
    ax1.grid(True, alpha=0.3)
    
    # SIR Model Results
    ax2 = axes[0, 1]
    ax2.plot(sir_results['time'], sir_results['S'], label='Susceptible', color='blue')
    ax2.plot(sir_results['time'], sir_results['I'], label='Infected', color='red')
    ax2.plot(sir_results['time'], sir_results['R'], label='Recovered', color='green')
    ax2.set_title('Classical SIR Model')
    ax2.set_xlabel('Time (days)')
    ax2.set_ylabel('Population')
    ax2.legend()
    ax2.grid(True, alpha=0.3)
    
    # Real COVID-19 Data
    ax3 = axes[1, 0]
    ax3.plot(country_data['Date'], country_data['Confirmed'], label='Confirmed', color='red')
    ax3.plot(country_data['Date'], country_data['Deaths'], label='Deaths', color='black')
    if 'Recovered' in country_data.columns:
        ax3.plot(country_data['Date'], country_data['Recovered'], label='Recovered', color='green')
    ax3.set_title(f'Real COVID-19 Data - {country}')
    ax3.set_xlabel('Date')
    ax3.set_ylabel('Cumulative Cases')
    ax3.legend()
    ax3.grid(True, alpha=0.3)
    plt.setp(ax3.xaxis.get_majorticklabels(), rotation=45)
    
    # Daily New Cases
    ax4 = axes[1, 1]
    ax4.plot(country_data['Date'], country_data['Daily_New_Cases'], color='orange', alpha=0.7)
    ax4.set_title(f'Daily New Cases - {country}')
    ax4.set_xlabel('Date')
    ax4.set_ylabel('New Cases')
    ax4.grid(True, alpha=0.3)
    plt.setp(ax4.xaxis.get_majorticklabels(), rotation=45)
    
    plt.tight_layout()
    plt.savefig(f'outbreak_analysis_{country}.png', dpi=300, bbox_inches='tight')
    plt.show()

if __name__ == "__main__":
    # Set random seeds for reproducibility
    np.random.seed(42)
    random.seed(42)
    
    # Run the complete analysis
    run_complete_analysis()