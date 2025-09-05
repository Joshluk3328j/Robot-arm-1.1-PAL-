# Constants
acc_d_gravity = 9.8          # Acceleration due to gravity (m/s^2)
g2kg = 1 / 1000              # Conversion from grams to kilograms
mm2m = 1 / 1000              # Conversion from millimeters to meters

def calc(
    l2_l: float, l2_mass: float,
    l3_l: float, l3_mass: float,
    l4_l: float, l4_mass: float,
    ee_l: float, ee_mass: float,
    servo2: float = 56, servo3: float = 13.4
) -> float:
    """
    Computes the total torque required at the shoulder joint due to link masses and servo weights.

    Parameters:
    - l2_l, l3_l, l4_l, ee_l: Lengths of links in mm
    - l2_mass, l3_mass, l4_mass, ee_mass: Masses of links in grams
    - servo2, servo3: Masses of servos in grams (servo2 is closer to base)

    Returns:
    - Total torque in Nm (printed)
    """

    # Masses and lengths of the links
    link_masses: list[float] = [l2_mass, l3_mass, l4_mass, ee_mass]
    link_lengths: list[float] = [l2_l, l3_l, l4_l, ee_l]

    # Compute gravitational forces (weights) in Newtons
    link_weights: dict[str, float] = {}
    for i, mass_g in enumerate(link_masses):
        link_weights[f"l{i+2}"] = (mass_g * g2kg) * acc_d_gravity  # F = m * g

    # Calculate torque contribution from each link mass
    link_torques: list[float] = []
    cumulative_distance: float = 0  # running total of distances from base

    for i, (_, weight) in enumerate(link_weights.items()):
        # Add half of the current link’s length to get to its CoG
        cumulative_distance += (link_lengths[i] / 2) * mm2m
        torque = weight * cumulative_distance
        link_torques.append(torque)

        # Add the remaining half to prepare for next link
        cumulative_distance += (link_lengths[i] / 2) * mm2m

    # Calculate torque contribution from servo masses (assuming mounted at joint ends)
    servo_torques: list[float] = []
    servo_distances_mm: list[float] = [
        link_lengths[0] + link_lengths[1] / 2,
        link_lengths[0] + link_lengths[1],
        link_lengths[0] + link_lengths[1] + link_lengths[2],
        link_lengths[0] + link_lengths[1] + link_lengths[2] + link_lengths[3],
    ]
    servo_distances_m: list[float] = [d * mm2m for d in servo_distances_mm]

    # Convert servo masses to forces (N)
    servo_weights: list[float] = [
        servo2 * g2kg * acc_d_gravity,
        servo3 * g2kg * acc_d_gravity,
        servo3 * g2kg * acc_d_gravity,
        servo3 * g2kg * acc_d_gravity,
    ]

    # Calculate torque from each servo
    for weight, distance in zip(servo_weights, servo_distances_m):
        torque = weight * distance
        servo_torques.append(torque)

    # Total torque from links and servos
    total_torque: float = sum(link_torques) + sum(servo_torques)
    print(f"Total torque from links and servos: {total_torque:.3f} Nm")
    return total_torque, sum(link_lengths)


def torque_mass_length(mass_g: float, length_mm: float) -> None:
    """
    Calculates torque produced by a point mass at a given distance.

    Parameters:
    - mass_g: Mass in grams
    - length_mm: Lever arm distance in mm
    """
    weight = mass_g * g2kg * acc_d_gravity
    distance_m = length_mm * mm2m
    torque = weight * distance_m
    print(f"Torque from {mass_g:.2f}g ({weight:.2f}N) at {length_mm}mm = {torque:.3f} Nm")


def servo_mass_distance(servo_rating_kg: float, distance_mm: float, percent: float = 100) -> None:
    """
    Estimates the maximum mass a servo can lift at a given distance, using a percentage of its max torque.

    Parameters:
    - servo_rating_kg: Torque rating in kg·cm
    - distance_mm: Lever arm distance in mm
    - percent: Percent of servo torque to use (e.g., 70%)
    """
    distance_cm = distance_mm / 10
    usable_torque_kgcm = servo_rating_kg * (percent / 100)
    usable_torque_Nm = (10 * usable_torque_kgcm) * acc_d_gravity * mm2m  # Convert kg·cm to Nm

    max_mass_kg = usable_torque_kgcm / distance_cm
    max_force_N = max_mass_kg * acc_d_gravity

    print(f"Using {percent}% of a {servo_rating_kg} kg·cm servo's torque:")
    print(f"→ Usable torque: {usable_torque_kgcm:.2f} kg·cm ≈ {usable_torque_Nm:.3f} Nm")
    print(f"→ Max load at {distance_cm:.1f} cm: {max_mass_kg:.3f} kg ({max_force_N:.2f} N)")


# Calculate total torque from full arm setup
torque, full_extension = calc(
    l2_l=100, l2_mass=50,
    l3_l=84.991, l3_mass=50,
    l4_l=41.647,  l4_mass=50,
    ee_l=119.535, ee_mass=50
)

# Check usable Torque full length using 70% torque
servo_mass_distance(11, full_extension, 70)

# # Get torque from 350g at the full length of all links
# torque_mass_length(350, full_extension)
