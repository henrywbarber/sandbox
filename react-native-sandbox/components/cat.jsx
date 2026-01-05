import React, { useState } from 'react';
import { Button, Text, View } from 'react-native';

const getFullName = (firstName, secondName, thirdName) => {
  return firstName + ' ' + secondName + ' ' + thirdName;
}

const Cat = props => {
  const [isHungry, setIsHungry] = useState(true);
  return (
		<View>
			<Text>
        I am {getFullName(props.first, props.second, props.third)}, and I am {isHungry ? 'hungry' : 'full'}!
      </Text>
      <Button
        onPress={() => {
          setIsHungry(false);
        }}
        disabled={!isHungry}
        title={isHungry ? 'Please feed me!' : 'Thank you!'}
      />
		</View>
	);
};

export default Cat;

