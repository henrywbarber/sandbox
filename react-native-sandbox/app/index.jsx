import { Button, Image, ScrollView, StyleSheet, Text, TextInput, View } from "react-native";

import { Link } from "expo-router";

import Cat from '../components/cat.jsx';
import PizzaTranslator from '../components/pizzaTranslator.jsx';
import { useApp } from "../context/AppContext";

export default function HomePage() {
	const { count, increment } = useApp();

	// forEach
	const fruits = ["apple", "banana", "cherry"];
	let fruitList = "Fruits: ";
	fruits.forEach(fruit => {
		fruitList += fruit + " ";
	});
	console.log(fruitList); // Output: "Fruits: apple banana cherry "

	// filter
	const ages = [12, 18, 25, 6];
	const adults = ages.filter(age => age >= 18);
	console.log(adults); // Output: [18, 25]

	// reduce
	const prices = [10.5, 5.0, 20.0];
	const total = prices.reduce((accumulator, currentValue) => {
		return accumulator + currentValue;
	}, 0); // 0 is the initial value for the accumulator
	console.log(total); // Output: 35.5

	// for ... of
	const colors = ["red", "green", "blue"];
	for (const color of colors) {
		// Iterate directly over the values
		console.log(`I like ${color}`);
	}

	// for ... in
	const user = { firstName: "Alice", age: 30 };
	for (const key in user) {
		// Iterate over object keys (arb order)
		console.log(`${key}: ${user[key]}`);
	}

	return (
		<View style={{ padding: 40 }}>
			<Text>Home Page</Text>
			<Text style={{ fontSize: 24 }}>Count: {count}</Text>

			<Button title="Increment" onPress={increment} />

			<ScrollView>
				<Text>Example Text</Text>
				<View>
					<Text>More nested text</Text>
					<Image
						source={{
							uri: "https://reactnative.dev/docs/assets/p_cat2.png"
						}}
						style={{ width: 200, height: 200 }}
					/>
				</View>
				<TextInput
					style={{
						height: 40,
						borderColor: "gray",
						borderWidth: 1
					}}
					defaultValue="You can type here..."
				/>
				<Text>Welcome to the Cat Cafe!</Text>
				<Cat first="Rum" second="Tum" third="Tiger" />
				<Cat first="Kitty" second="Cat" third="Kit" />
				<Text>Pizza Translator:</Text>
				<PizzaTranslator />
				{/* <ExampleScrollable /> */}
				{/* <ExampleSectionlist /> */}
			</ScrollView>

			<Link href="/page2">
				<Text style={[ styles.pageLinkText ]}>Go to Page 2</Text>
			</Link>
		</View>
	);
}

const styles = StyleSheet.create({
	pageLinkText: {
		marginTop: 20,
		color: "blue"
	},
});
