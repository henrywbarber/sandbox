import { View, Text, Button } from "react-native";

import { useRouter } from "expo-router";

import { useApp } from "../context/AppContext";

export default function Page2() {
	const { count, decrement } = useApp();
	const router = useRouter();

	return (
		<View style={{ padding: 40 }}>
			<Text>Page 2</Text>
			<Text style={{ fontSize: 24 }}>Count: {count}</Text>

			<Button title="Decrement" onPress={decrement} />

			<Button title="Back" onPress={() => router.back()} />
		</View>
	);
}
